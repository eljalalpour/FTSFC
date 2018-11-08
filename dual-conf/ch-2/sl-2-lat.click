// In aqua07
// aqua07 -> aqua09 -> aqua10
// aqua10 -> aqua09 -> aqua01

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice(0,0);
    td1::ToDPDKDevice(0,0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.2.0.0/16)
    -> FTMBInputLogger()
    -> FTMBMaster(PERIOD 200, DELAY 6, PER_PACKET 3)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(192.168.1.110, dst)
    -> StoreEtherAddress(0c:c4:7a:73:f9:ec, src) // from aqua09
    -> StoreEtherAddress(0c:c4:7a:73:fa:18, dst) // to aqua10
    -> td1;
}

MBBlock(0, 1.3.3.3);
