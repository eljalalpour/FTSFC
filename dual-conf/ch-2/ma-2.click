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
    -> FTMBMaster(PERIOD 200, DELAY 6, PER_PACKET 10)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(10.70.0.9, dst)
    -> StoreEtherAddress(e4:1d:2d:0c:89:e0, src) // from aqua10
    -> StoreEtherAddress(e4:1d:2d:13:9c:60, dst) // to aqua09
    -> td1;
}

MBBlock(0, 1.5.5.5);
