// In aqua08
// aqua07 -> aqua08 -> aqua07

array::LockFreeArray;

elementclass MBBlock {
$index,$src_ip |
    fd1::FromDPDKDevice(1,0);
    td1::ToDPDKDevice(1,0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.4.0.0/16)
//    -> IPPrint("from slave 2")
    -> FTMBMaster(PERIOD 200, DELAY 6, PER_PACKET 5)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(192.168.1.109, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:18, src) // from aqua10
    -> StoreEtherAddress(0c:c4:7a:73:f9:ec, dst) // to aqua09
//    -> IPPrint("To slave 2")
    -> td1;
}

MBBlock(0, 1.5.5.5);
