// In aqua04
// aqua03 -> aqua04 -> aqua05

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice($dev, 0);
    td1::ToDPDKDevice($dev, 0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.8.0.0/16)
    -> FTMBInputLogger()
    -> FTMBMaster(PERIOD 200, DELAY 6, PER_PACKET 4)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)   // from aqua 04
    -> StoreIPAddress(10.70.0.5, dst) // to aqua 05
    -> StoreEtherAddress(e4:1d:2d:13:9f:e0, src) // from aqua 04
    -> StoreEtherAddress(e4:1d:2d:13:9e:b0, dst) // to aqua 05
    -> td1;
}

MBBlock(1, 1.9.1.1);
