// In aqua08
// aqua07 -> aqua08 -> aqua07


array::LockFreeArray;

elementclass MBBlock {
$index,$src_ip |
    fd1::FromDPDKDevice(0,0);
    td1::ToDPDKDevice(0,0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
//    -> IPPrint("from slave-1")
    -> FTMBMaster(PERIOD 200, DELAY 6, PER_PACKET 5)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(192.168.1.107, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, src) // from aqua08
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, dst) // to aqua07
//    -> IPPrint("to slave-1")
    -> td1;
}

MBBlock(0, 1.2.2.2);
