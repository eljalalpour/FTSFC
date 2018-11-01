// In aqua07
// aqua01 -> aqua07 -> aqua08

elementclass NFBlock {
$index,$src_ip |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.0.0.0/16)
//    -> IPPrint($index)
    -> FTMBSim(PERIOD 200, DELAY 6, PER_PACKET 20)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(10.70.0.8, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
    -> StoreEtherAddress(f4:52:14:5a:90:70, dst)
    -> output
}

fd1::FromDPDKDevice(1,0);
td1::ToDPDKDevice(1,0);

fd1
-> NFBlock(0,1.1.1.1)
-> c1::Counter
-> td1;
