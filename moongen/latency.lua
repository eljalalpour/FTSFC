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
local DST_MAC		= nil -- resolved via ARP on GW_IP or DST_IP, can be overriden with a string here
local SRC_IP_BASE	= "192.168.1.101" -- actual address will be SRC_IP_BASE + random(0, flows)
local DST_IP		= "192.168.1.107"
local SRC_PORT		= 1234
local DST_PORT		= 319

-- answer ARP requests for this IP on the rx port
-- change this if benchmarking something like a NAT device
local RX_IP		= DST_IP
-- used to resolve DST_MAC
local GW_IP		= DST_IP
-- used as source IP to resolve GW_IP to DST_MAC
local ARP_IP	= SRC_IP_BASE

function configure(parser)
    parser:description("Generates UDP traffic and measure latencies. Edit the source to modify constants like IPs.")
    parser:option("txDev", "Device to transmit from."):default(0):convert(tonumber)
    parser:option("rxDev", "Device to receive from."):default(0):convert(tonumber)
    parser:option("-r --rate", "Transmit rate in Mbit/s."):default(10000):convert(tonumber)
    parser:option("-f --flows", "Number of flows (randomized source IP)."):default(4):convert(tonumber)
    parser:option("-s --size", "Packet size."):default(1024):convert(tonumber)
    parser:option("-o --out", "Filename of the latency histogram."):default("latency.csv")
    parser:option("-d --duration", "Experiment duration"):default(20)
end

function master(args)
    txDev = device.config{port = args.txDev, rxQueues = 3, txQueues = 3}
    rxDev = device.config{port = args.rxDev, rxQueues = 3, txQueues = 3}
    device.waitForLinks()
    -- max 1kpps timestamping traffic timestamping
    -- rate will be somewhat off for high-latency links at low rates
    if args.rate > 0 then
        txDev:getTxQueue(0):setRate(args.rate - (args.size + 4) * 8 / 1000)
    end

    mg.startTask("timerSlave",
            txDev:getTxQueue(1), rxDev:getRxQueue(1),
            args.size, args.flows,
            args.dur_timeout, args.out)
    --arp.startArpTask{
    --    -- run ARP on both ports
    --    { rxQueue = rxDev:getRxQueue(2), txQueue = rxDev:getTxQueue(2), ips = RX_IP },
    --    -- we need an IP address to do ARP requests on this interface
    --    { rxQueue = txDev:getRxQueue(2), txQueue = txDev:getTxQueue(2), ips = ARP_IP }
    --}
    mg.waitForTasks()
end

local function fillUdpPacket(buf, len)
    buf:getUdpPacket():fill{
        ethSrc = queue,
        ethDst = DST_MAC,
        ip4Src = SRC_IP,
        ip4Dst = DST_IP,
        udpSrc = SRC_PORT,
        udpDst = DST_PORT,
        pktLength = len
    }
end

local function doArp()
    if not DST_MAC then
        log:info("Performing ARP lookup on %s", GW_IP)
        DST_MAC = arp.blockingLookup(GW_IP, 5)
        if not DST_MAC then
            log:info("ARP lookup failed, using default destination mac address")
            return
        end
    end
    log:info("Destination mac: %s", DST_MAC)
end


function timerSlave(txQueue, rxQueue, size, flows, duration, out)
    doArp()
    if size < 84 then
        log:warn("Packet size %d is smaller than minimum timestamp size 84. Timestamped packets will be larger than load packets.", size)
        size = 84
    end
    local timestamper = ts:newUdpTimestamper(txQueue, rxQueue)
    local hist = hist:new()
    mg.sleepMillis(1000) -- ensure that the load task is running
    local counter = 0
    local rateLimit = timer:new(0.001)
    local baseIP = parseIPAddress(SRC_IP_BASE)
    local dur_timeout = timer:new(duration)
    dur_timeout.reset()
    while mg.running() and dur_timeout:running() do
        hist:update(timestamper:measureLatency(size, function(buf)
            fillUdpPacket(buf, size)
            local pkt = buf:getUdpPacket()
            pkt.ip4.src:set(baseIP + counter)
            counter = incAndWrap(counter, flows)
        end))
        rateLimit:wait()
        rateLimit:reset()
    end
    -- print the latency stats after all the other stuff
    mg.sleepMillis(300)
    hist:print()
    hist:save(out)
end