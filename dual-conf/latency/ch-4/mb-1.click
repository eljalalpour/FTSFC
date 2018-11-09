// In aqua07
// aqua01 -> aqua07 -> aqua08

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice($dev, 0);
    td1::ToDPDKDevice($dev, 0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.0.0.0/16)
    -> FTMBInputLogger()
    -> FTMBMaster(PERIOD 200, DELAY 6, PER_PACKET 3)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)   // from aqua 7
    -> StoreIPAddress(192.168.1.108, dst) // to aqua 8
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, src) // from aqua 7
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst) // to aqua 8
    -> td1;
}

MBBlock(1, 1.1.1.1);
