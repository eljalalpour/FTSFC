package.path = package.path ..";?.lua;test/?.lua;app/?.lua;../?.lua"

require "Pktgen";

local DEVICE = "0";
local RATE = 100;
local SIZE = 256;

local SRC_MAC  = "f4:52:14:69:a5:20"; -- Aqua01
local DST_MAC  = "e4:1d:2d:13:9e:d0"; -- Aqua07
local SRC_IP = "1.0.1.1"
local SRC_IP_MIN = "1.0.0.0"
local SRC_IP_MAX = "1.0.255.255"
local SRC_IP_INC = "0.0.0.1"

local VARIABLE_PORTS = true

local SRC_PORT_MIN = 1024
local SRC_PORT_MAX = 65535
local SRC_PORT_INC = 1

local DST_PORT_MIN = 1024
local DST_PORT_MAX = 65535
local DST_PORT_INC = 1

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
    pktgen.pkt_size(dev, "start", pkt_size);

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

    if VARIABLE_PORTS == true then
        pktgen.dst_port(dev, "start", DST_PORT_MIN);
        pktgen.dst_port(dev, "inc", DST_PORT_INC);
        pktgen.dst_port(dev, "min", DST_PORT_MIN);
        pktgen.dst_port(dev, "max", DST_PORT_MAX);

        pktgen.src_port(dev, "start", SRC_PORT_MIN);
        pktgen.src_port(dev, "inc", SRC_PORT_INC);
        pktgen.src_port(dev, "min", SRC_PORT_MIN);
        pktgen.src_port(dev, "max", SRC_PORT_MAX);
    end

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

