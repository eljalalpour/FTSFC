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
    -> FTMBMaster(PERIOD 200, DELAY 6, PER_PACKET 3)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)   // from aqua 04
    -> StoreIPAddress(192.168.1.105, dst) // to aqua 05
    -> StoreEtherAddress(0c:c4:7a:73:fa:20, src) // from aqua 04
    -> StoreEtherAddress(0c:c4:7a:73:fa:6e, dst) // to aqua 05
    -> td1;
}

MBBlock(0, 1.9.1.1);
