// In aqua11
// aqua10 -> aqua11 -> aqua12

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice($dev, 0);
    td1::ToDPDKDevice($dev, 0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.4.0.0/16)
    -> FTMBInputLogger()
    -> FTMBMaster(PERIOD 200, DELAY 6, PER_PACKET 4)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)    // from aqua 11
    -> StoreIPAddress(10.70.0.12, dst) // to aqua 12
    -> StoreEtherAddress(e4:1d:2d:13:9e:e0, src) // from aqua 11
    -> StoreEtherAddress(e4:1d:2d:0c:89:d0, dst) // to aqua 12
    -> td1;
}

MBBlock(0, 1.5.1.1);
