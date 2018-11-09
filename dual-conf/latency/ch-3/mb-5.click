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
    -> FTMBMaster(PERIOD 200, DELAY 6, PER_PACKET 3)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)    // from aqua 11
    -> StoreIPAddress(192.168.1.112, dst) // to aqua 12
    -> StoreEtherAddress(0c:c4:7a:73:fa:e6, src) // from aqua 11
    -> StoreEtherAddress(0c:c4:7a:73:fa:9e, dst) // to aqua 12
    -> td1;
}

MBBlock(1, 1.5.1.1);
