// In aqua09
// aqua08 -> aqua09 -> aqua10

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice($dev, 0);
    td1::ToDPDKDevice($dev, 0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.2.0.0/16)
    -> FTMBInputLogger()
    -> FTMBMaster(PERIOD 200, DELAY 6, PER_PACKET 4)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)    // from aqua 9
    -> StoreIPAddress(10.70.0.10, dst) // to aqua 10
    -> StoreEtherAddress(e4:1d:2d:13:9c:60, src) // from aqua 9
    -> StoreEtherAddress(e4:1d:2d:0c:89:e0, dst) // to aqua 10
    -> td1;
}

MBBlock(0, 1.3.1.1);
