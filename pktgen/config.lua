package.path = package.path ..";?.lua;test/?.lua;app/?.lua;../?.lua"

require "Pktgen";

local DEVICE = "0";
local RATE = 100;
local SIZE = 1350;

local SRC_MAC  = "f4:52:14:69:a5:20"; -- Aqua01
local DST_MAC  = "e4:1d:2d:13:9e:d0"; -- Aqua07
local SRC_IP = "1.0.1.1"
local SRC_IP_MIN = "1.0.1.1"
local SRC_IP_MAX = "1.0.254.254"
local SRC_IP_INC = "0.0.0.1"
local NET_MASK = "/16";
local DST_IP   = "10.70.0.7";

function configure()
    local args = {
        ["dev"] = DEVICE,
        ["rate"] = RATE,
        ["size"] = SIZE,
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

end

function main()
    local args = configure();
    set_options(args.dev, args.rate, args.size);
end

main();

