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
local DST_MAC  = "0c:c4:7a:73:fa:54" -- resolved via ARP on GW_IP or DST_IP, can be overriden with a string here
local SRC_IPS  = {
    "192.168.1.101",
    "192.168.1.2",
    "192.168.1.3",
    "192.168.1.4",
    "192.168.1.5",
    "192.168.1.6",
    "192.168.1.7",
    "192.168.1.8",
} -- actual address will be SRC_IP_BASE + random(0, flows)
local DST_IP   = "192.168.1.107"
local SRC_PORT = 1234
local DST_PORT = 319

-- answer ARP requests for this IP on the rx port
-- change this if benchmarking something like a NAT device
local RX_IP		= DST_IP
-- used to resolve DST_MAC
local GW_IP		= DST_IP
-- used as source IP to resolve GW_IP to DST_MAC
local ARP_IP	= SRC_IP

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
    local ii = math.random(1,#SRC_IPS)
    buf:getUdpPacket():fill{
        ethSrc = SRC_MAC,
        ethDst = DST_MAC,
        ip4Src = SRC_IPS[ii],
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
