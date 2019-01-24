local mg     = require "moongen"
local memory = require "memory"
local device = require "device"
local ts     = require "timestamping"
local filter = require "filter"
local hist   = require "histogram"
local stats  = require "stats"
local timer  = require "timer"
local arp    = require "proto.arp"
local log    = require "log"

-- set addresses here
local SRC_MAC  = "0c:c4:7a:73:fa:72"
local DST_MAC  = "0c:c4:7a:73:fa:54"
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

local RATE_LIMIT_TIMER = 0.001

function configure(parser)
    parser:description("Generates UDP traffic and measure latencies. Edit the source to modify constants like IPs.")
    parser:option("-d --dev", "Device to transmit/receive from."):default(0):convert(tonumber)
    parser:option("-r --rate", "Transmit rate in Mbit/s."):default(10000):convert(tonumber)
    parser:option("-s --size", "Packet size."):default(1000):convert(tonumber)
    parser:option("-o --out", "Filename of the latency histogram."):default("latency.csv")
    parser:option("-l --lat", "Filename of the latency over time."):default("latency-overtime.csv")
    parser:option("-d --duration", "Experiment duration (in seconds)"):default(10):convert(tonumber)
end

function master(args)
    local dev = device.config({port = args.dev, rxQueues = 2, txQueues = 2, bufSize = 9000, numBufs = 1024})
    device.waitForLinks()

    -- max 1kpps timestamping traffic timestamping
    -- rate will be somewhat off for high-latency links at low rates
    if args.rate > 0 then
        dev:getTxQueue(0):setRate(args.rate)
        dev:getTxQueue(1):setRate(args.rate)
    end

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