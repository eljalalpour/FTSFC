package.path = package.path ..";?.lua;test/?.lua;app/?.lua;../?.lua"

require "Pktgen";
local argparse = require "argparse";

local SCREEN_OPTION = "on";
-- set addresses here
local SRC_MAC  = "f4:52:14:69:a5:20"; -- Aqua01
local DST_MAC  = "e4:1d:2d:13:9e:d0"; -- Aqua07
local SRC_IP = "1.0.1.1"
local SRC_IP_MIN = "1.0.1.1"
local SRC_IP_MAX = "1.0.255.255"
local SRC_IP_INC = "0.0.0.1"
local NET_MASK = "/8";
local DST_IP   = "10.70.0.7";
local DELAY = 20000;
local SAMPLE_PERIOD = 500;
local LINK_CAP = 40000;

function configure()
    local args = {
        ["dev"] = 0,
        ["rate"] = 1,
        ["size"] = 1000,
        ["duration"] = 10,
        ["out"] = "throughput.csv"
    };

    return args
end

local function set_options(dev, rate, pkt_size)
    pktgen.screen(SCREEN_OPTION);
    pktgen.set_ipaddr(dev, "src", SRC_IP..NET_MASK);
    pktgen.set_ipaddr(dev, "dst", DST_IP);
    pktgen.set_mac(dev, DST_MAC);
    pktgen.set_proto(dev ..",".. dev, "udp");
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

    set_options(args.dev, args.rate, args.size)

    pktgen.stop(args.dev);
    sleep(2);					-- Wait for stop to happen (not really needed)

    send(args.dev, args.duration, args.output)

    printf("Done!\n");
end

main();





