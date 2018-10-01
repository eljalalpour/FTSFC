-- CGCS Demo script
package.path = package.path ..";?.lua;test/?.lua;app/?.lua;../?.lua"

local pktgen = require "Pktgen";
local argparse = require "argparse"

local SCREEN_OPTION = "on"
-- set addresses here
local SRC_MAC  = "0c:c4:7a:73:fa:72"
local DST_MAC  = "0c:c4:7a:73:fa:54"
local SRC_IPS  = {
    "1.0.1.1",
    "1.0.2.2",
    "1.0.3.3",
    "1.0.4.4",
    "1.0.5.5",
    "1.0.6.6",
    "1.0.7.7",
    "1.0.8.8",
}
local SRC_IP_INDEX = 0
local SRC_IP   = "1.0.1.1"
local NET_MASK = "/8"
local DST_IP   = "10.70.0.7"
local SRC_PORT = 1234
local DST_PORT = 4321
local DELAY = 20000
local SAMPLE_PERIOD = 500

function configure()
    local parser = argparse()

    parser:description("Generates UDP traffic and measure throughput.")
    parser:option("-d --dev", "Device to transmit/receive from."):default(0):convert(tonumber)
    parser:option("-r --rate", "Transmit rate in Mbit/s."):default(10000):convert(tonumber)
    parser:option("-s --size", "Packet size."):default(1000):convert(tonumber)
    parser:option("-d --duration", "Experiment duration (in seconds)"):default(10):convert(tonumber)
    parser:option("-o --out", "Filename of the throughput report."):default("throughput.csv")

    return parser.parse()
end

local function default_options(send_dev, recv_dev)
    pktgen.screen(SCREEN_OPTION);
    pktgen.set_ipaddr(send_dev, "src", SRC_IP..NET_MASK);
    pktgen.set_ipaddr(send_dev, "dst", DST_IP);
    pktgen.set_mac(send_dev, DST_MAC);
    pktgen.set_proto(send_dev ..",".. recv_dev, "udp");
end

local function set_options(dev, rate, pkt_size)
    pktgen.set(dev, "rate", rate);
    pktgen.set(dev, "size", pkt_size);
end

local function send(dev, duration, output)
    local report_file = io.open(output, "w")
    pktgen.start(dev);
    printf("Waiting for device to come up!")
    pktgen.delay(DELAY);

    io.output(report_file)
    for i = 1, duration,1
    do
        io.write(
                pktgen.portStats("all", "rate")[0]["pkts_rx"], ",",
                pktgen.portStats("all", "rate")[0]["mbits_rx"], "\n");
        pktgen.delay(SAMPLE_PERIOD);
    end
    io.close(report_file);

    pktgen.stop("all");
end


function main()
    local args = configure()
    printf("Running at rate %d and packet-size %d for %d seconds!\n", args.rate, args.size, args.duration);

    default_options(args.dev, args.dev)
    set_options(args.dev, args.send_rate, args.pkt_size)
    send(args.dev, args.duration, args.output)

    printf("Done!\n");
end

main();





