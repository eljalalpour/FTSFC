local mg     = require "moongen"
local memory = require "memory"
local device = require "device"
local ts     = require "timestamping"
local stats  = require "stats"
local hist   = require "histogram"
local timer  = require "timer"

-- set addresses here
local SRC_MAC = "0c:c4:7a:73:fa:72"
local DST_MAC = "0c:c4:7a:73:fa:54"
local SRC_IP_FORMAT_STR = '1.0.%d.%d'
local SRC_IPS  = { }

local IP_BEG = 0
local IP_END = 255

local index = 0
for i = IP_BEG, IP_END do
    for j = IP_BEG, IP_END do
        SRC_IPS[index] = SRC_IP_FORMAT_STR:format(i, j)
        index = index + 1
    end
end
local SRC_IP_INDEX = 0
local DST_IP   = "192.168.1.107"
local SRC_PORT = 1234
local DST_PORT = 4321


local PKT_SIZE	= 60

local function getRstFile(...)
    local args = { ... }
    for i, v in ipairs(args) do
        result, count = string.gsub(v, "%-%-result%=", "")
        if (count == 1) then
            return i, result
        end
    end
    return nil, nil
end

function configure(parser)
    parser:description("Generates bidirectional CBR traffic with hardware rate control and measure latencies.")
    parser:option("-d --dev", "Device to transmit/receive from."):default(0):convert(tonumber)
    parser:option("-r --rate", "Transmit rate in Mbit/s."):default(10000):convert(tonumber)
    parser:option("-o --out", "Filename of the latency histogram."):default("histogram.csv")
    parser:option("-l --lat", "Filename of the latency over time."):default("overtime.csv")
    parser:option("-s --size", "Packet size."):default(1000):convert(tonumber)
    parser:option("-d --duration", "Experiment duration (in seconds)"):default(10):convert(tonumber)
end

function master(args)
    local dev = device.config({port = args.dev, rxQueues = 3, txQueues = 3, bufSize = 9000, numBufs = 1024})

    device.waitForLinks()

    -- max 1kpps timestamping traffic timestamping
    -- rate will be somewhat off for high-latency links at low rates
    if args.rate > 0 then
        dev:getTxQueue(0):setRate(args.rate)
        dev:getTxQueue(1):setRate(args.rate)
        dev:getTxQueue(2):setRate(args.rate)
    end

    math.randomseed(os.time())
    mg.startTask("loadSlave",
            dev:getTxQueue(0),
            dev:getRxQueue(0),
            args.size,
            args.duration)

    mg.startTask("timerSlave",
            dev:getTxQueue(1),
            dev:getRxQueue(1),
            args.size,
            args.duration,
            args.out,
            args.lat)

    mg.waitForTasks()
    mg.stop()
end

local function fillUdpPacket(buf, len)
    SRC_IP_INDEX = (SRC_IP_INDEX + 1) % (#SRC_IPS)
    buf:getUdpPacket():fill{
        ethSrc = SRC_MAC,
        ethDst = DST_MAC,
        ip4Src = SRC_IPS[SRC_IP_INDEX],
        ip4Dst = DST_IP,
        udpSrc = SRC_PORT,
        udpDst = DST_PORT,
        pktLength = len
    }
end

function loadSlave(queue, rxDev, size, duration)
    local mempool = memory.createMemPool(function(buf)
        fillUdpPacket(buf, size)
    end)
    local bufs = mempool:bufArray()
    local txCtr = stats:newDevTxCounter(queue, "plain")
    local rxCtr = stats:newDevRxCounter(rxDev, "plain")

    mg.sleepMillis(5000) -- ensure that the load task is running

    --local thr_hist = hist:new()
    local dur_timeout = timer:new(duration)
    while mg.running() and dur_timeout:running() do
        -- UDP checksums are optional, so using just IPv4 checksums would be sufficient here
        bufs:alloc(size)
        bufs:offloadUdpChecksums()
        queue:send(bufs)
        txCtr:update()
        rxCtr:update()
    end
    txCtr:finalize()
    rxCtr:finalize()

    -- print the latency stats after all the other stuff
    mg.sleepMillis(300)
end

local function store_latency_over_time(lat_over_time, file_path)
    local myfile = io.open(file_path, "w")

    for i=1, #lat_over_time do
        myfile:write(tostring(lat_over_time[i])..',')
    end

    myfile:close()
end

function timerSlave(txQueue, rxQueue, size, duration, hist_out, overtime_out)
    local timestamper = ts:newUdpTimestamper(txQueue, rxQueue)
    local hist = hist:new()
    local rateLimit = timer:new(RATE_LIMIT_TIMER)

    local lat_over_time = {}

    mg.sleepMillis(5000) -- ensure that the load task is running
    local durTimeout = timer:new(duration)

    while mg.running() and durTimeout:running() do
        local lat = timestamper:measureLatency(size,
                function(buf)
                    fillUdpPacket(buf, size)
                end)
        hist:update(lat)
        lat_over_time[#lat_over_time + 1] = lat
        rateLimit:busyWait()
        rateLimit:reset()
    end
    -- print the latency stats after all the other stuff
    mg.sleepMillis(300)
    hist:print()
    hist:save(hist_out)
    store_latency_over_time(lat_over_time, overtime_out)
end
