package.path = package.path ..";?.lua;test/?.lua;app/?.lua;../?.lua"

require "Pktgen";

local DEVICE = "0";
local RATE = 100;
local SIZE = 1000;
local DURATION = 10;
local OUTPUT_FILE = "throughput.csv";

local SRC_MAC  = "f4:52:14:69:a5:20"; -- Aqua01
local DST_MAC  = "e4:1d:2d:13:9e:d0"; -- Aqua07
local SRC_IP = "1.0.1.1"
local SRC_IP_MIN = "1.0.1.1"
local SRC_IP_MAX = "1.0.254.254"
local SRC_IP_INC = "0.0.0.1"
local NET_MASK = "/16";
local DST_IP   = "10.70.0.7";
local DELAY = 2000;
local SAMPLE_PERIOD = 1000;

function configure()
    local args = {
        ["dev"] = DEVICE,
        ["rate"] = RATE,
        ["size"] = SIZE,
        ["duration"] = DURATION,
        ["out"] = OUTPUT_FILE
    };

    return args
end

function set_options(dev, rate, pkt_size)
    pktgen.ip_proto(dev, "udp");    
    pktgen.src_mac(dev, "start", SRC_MAC);
    pktgen.dst_mac(dev, "start", DST_MAC);

    pktgen.src_ip(dev, "start", SRC_IP);
    pktgen.src_ip(dev, "inc", SRC_IP_INC);
    pktgen.src_ip(dev, "min", SRC_IP_MIN);
    pktgen.src_ip(dev, "max", SRC_IP_MAX);
    
    pktgen.dst_ip(dev, "start", DST_IP);
    pktgen.dst_ip(dev, "inc", "0.0.0.0");
    pktgen.dst_ip(dev, "min", DST_IP);
    pktgen.dst_ip(dev, "max", DST_IP);
    
    pktgen.pkt_size(dev, "start", pkt_size);

    pktgen.set_ipaddr(dev, "src", SRC_IP..NET_MASK);
    pktgen.set_ipaddr(dev, "dst", DST_IP);
    pktgen.set_mac(dev, DST_MAC);
    pktgen.set_proto(dev ..",".. dev, "udp");
    pktgen.set(dev, "rate", rate);
    pktgen.set(dev, "size", pkt_size);

    pktgen.set_range(dev, "on")
    pktgen.process(dev, "on");

    pktgen.delay(DELAY);

end

function send(dev, duration, out)
    printf("Report results in file \"%s\"!\n", out);
    report_file = io.open(out, "w");
    --pktgen.port(0);
    pktgen.set_range(dev, "on")
    pktgen.process(dev, "on");
    pktgen.start(dev);
    printf("Waiting for device to come up!\n");
    pktgen.delay(DELAY);

    printf("Sending traffic...\n");
    io.output(report_file);
    for i = 1, duration, 1
    do
        io.write(
                pktgen.portStats(dev, "rate")[0]["pkts_rx"], ",",
                pktgen.portStats(dev, "rate")[0]["mbits_rx"], "\n");
        pktgen.delay(SAMPLE_PERIOD);
    end
    io.output(io.stdout);
    io.close(report_file);

    pktgen.stop("all");
end

function main()
    print("Throughput test...");
    local args = configure();
    printf("Running at rate %d percent and packet-size %d for %d seconds!\n", args.rate, args.size, args.duration);

    set_options(args.dev, args.rate, args.size);
    send(args.dev, args.duration, args.out);

    printf("Done!\n");
    pktgen.delay(DELAY);
    pktgen.quit();
end

main();

