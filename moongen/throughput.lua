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
-- set addresses here
local SRC_MAC  = "0c:c4:7a:73:fa:72"
local DST_MAC  = "0c:c4:7a:73:fa:54"
local SRC_IPS  = {
    "1.1.1.1",
    "1.1.2.2",
    "1.1.3.3",
    "1.1.4.4",
    "1.1.5.5",
    "1.1.6.6",
    "1.1.7.7",
    "1.1.8.8",
}
local SRC_IP_INDEX = 0
local DST_IP   = "192.168.1.107"
local SRC_PORT = 1234
local DST_PORT = 319

function configure(parser)
    parser:description("Generates UDP traffic and measure latencies. Edit the source to modify constants like IPs.")
    parser:option("-d --dev", "Device to transmit/receive from."):default(0):convert(tonumber)
    parser:option("-r --rate", "Transmit rate in Mbit/s."):default(10000):convert(tonumber)
    parser:option("-s --size", "Packet size."):default(1000):convert(tonumber)
    --parser:option("-o --out", "Filename of the latency histogram."):default("latency.csv")
    parser:option("-d --duration", "Experiment duration"):default(10):convert(tonumber)
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
    
    math.randomseed(os.time())
    mg.startTask("loadSlave",
            dev:getTxQueue(0),
            dev:getRxQueue(0),
            args.size,
            args.duration)
            --args.duration,
            --args.out)
    mg.waitForTasks()
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

--function loadSlave(queue, rxDev, size, duration, out)
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
        --thr_hist:update(txCtr)
        --thr_hist:update(rxCtr)
    end
    txCtr:finalize()
    rxCtr:finalize()

    -- print the latency stats after all the other stuff
    mg.sleepMillis(300)
    --thr_hist:print()
    --thr_hist:save(out)
end
