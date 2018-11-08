// In aqua02
// aqua12 -> aqua02 -> aqua03

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice($dev, 0);
    td1::ToDPDKDevice($dev, 0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.6.0.0/16)
    -> FTMBInputLogger()
    -> FTMBMaster(PERIOD 200, DELAY 6, PER_PACKET 3)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)    // from aqua 02
    -> StoreIPAddress(10.70.0.3, dst) // to aqua 03
    -> StoreEtherAddress(e4:1d:2d:13:9c:30, src) // from aqua 02
    -> StoreEtherAddress(e4:1d:2d:13:9f:80, dst) // to aqua 03
    -> td1;
}

MBBlock(1, 1.7.1.1);
