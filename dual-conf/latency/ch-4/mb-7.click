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
    -> StoreIPAddress(192.168.1.103, dst) // to aqua 03
    -> StoreEtherAddress(0c:c4:7a:73:fa:dc, src) // from aqua 02
    -> StoreEtherAddress(0c:c4:7a:73:fa:26, dst) // to aqua 03
    -> td1;
}

MBBlock(1, 1.7.1.1);
