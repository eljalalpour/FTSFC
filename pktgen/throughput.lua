-- CGCS Demo script
package.path = package.path ..";?.lua;test/?.lua;app/?.lua;../?.lua"

require "Pktgen";

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

function configure(parser)
    parser:description("Generates UDP traffic and measure throughput. Edit the source to modify constants like IPs.")
    parser:option("-d --dev", "Device to transmit/receive from."):default(0):convert(tonumber)
    parser:option("-r --rate", "Transmit rate in Mbit/s."):default(10000):convert(tonumber)
    parser:option("-s --size", "Packet size."):default(1000):convert(tonumber)
    parser:option("-d --duration", "Experiment duration (in seconds)"):default(10):convert(tonumber)
end

local function default_options(sendport, recvport)
    pktgen.screen(SCREEN_OPTION);
    pktgen.set_ipaddr(sendport, "src", SRC_IP..NET_MASK);
    pktgen.set_ipaddr(sendport, "dst", DST_IP);
    pktgen.set_mac(sendport, DST_MAC);
    pktgen.set_proto(sendport..","..recvport, "udp");
end

local function set_options(sendport, rate, pktSize)
    pktgen.set(sendport, "rate", rate);
    pktgen.set(sendport, "size", pktSize);
end

local function send(sendport, sendrate, pkt_size, seconds)
    local report_file = io.open("report.csv", "w")
    io.output(report_file)

    pktgen.start(sendport);

    set_options(sendport, sendrate, pkt_size)

    --printf("After start. delay for %d seconds\n", durationMs);
    pktgen.delay(20000);
    --prints("pktStats", pktgen.pktStats("all"));
    --prints("portRates", pktgen.portStats("all", "rate"));
    for i = 1,seconds,1
    do
        io.write(
                pktgen.portStats("all","rate")[0]["pkts_rx"],",",
                pktgen.portStats("all","rate")[0]["mbits_rx"],"\n");
        pktgen.delay(600);
    end

    pktgen.stop("all");

    pktgen.clear("all");

    io.close(report_file);
end


function main()


    rate = 100; size = 64; seconds = 60;
    printf("Runnning at rate %d, size %d, for %d secs\n", rate, size, seconds);
    send(rate, size, seconds);
    io.output(io.stdout);

    local sending = 0;
    local trlst = Set(time_step, pcnt_rate);

    -- Stop the port sending and reset to
    pktgen.stop(sendport);
    sleep(2);					-- Wait for stop to happen (not really needed)

    -- Set up the default packet size fixed value for now.
    pktgen.set(sendport, "size", pkt_size);

    pktgen.set_ipaddr(sendport, "dst", dstip);
    pktgen.set_ipaddr(sendport, "src", srcip..netmask);

    pktgen.set_proto(sendport..","..recvport, "udp");

    total_time = 0;
    -- v is the table to values created by the Set(x,y) function
    for _,v in pairs(trlst) do

        printf("   Percent load %d for %d seconds\n", v.rate, v.timo);

        -- Set the rate to the new value
        pktgen.set(sendport, "rate", v.rate);

        -- If not sending packets start sending them
        if ( sending == 0 ) then
            pktgen.start(sendport);
            sending = 1;
        end

        -- Sleep until we need to move to the next rate and timeout
        sleep(v.timo);
        total_time = total_time + v.timo;

    end

    -- Stop the port and do some cleanup
    pktgen.stop(sendport);
    sending = 0;

    printf("done\n");
end

main();





