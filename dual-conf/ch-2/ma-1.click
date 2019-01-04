// In aqua08
// aqua07 -> aqua08 -> aqua07

_array::SharedArray;

elementclass MBBlock {
$index,$src_ip |
    fd1::FromDPDKDevice(1,0);
    td1::ToDPDKDevice(1,0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
//    -> IPPrint("from slave-1")
    -> FTMBMaster(PERIOD 200, DELAY 6, PER_PACKET 5)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(10.70.0.7, dst)
    -> StoreEtherAddress(f4:52:14:5a:90:70, src) // from aqua08
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, dst) // to aqua07
//    -> IPPrint("to slave-1")
    -> td1;
}

MBBlock(0, 1.2.2.2);
