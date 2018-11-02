// In aqua07
// aqua01 -> aqua07 -> aqua08
// aqua08 -> aqua07 -> aqua09

elementclass MBBlock {
$index,$src_ip1,$src_ip2 |
    fd1::FromDPDKDevice(1,0);
    td1::ToDPDKDevice(1,0);
    td2::ToDPDKDevice(1,1);

    StaticThreadSched(td1 0, td2 1);

    fd1
    -> MarkIPHeader(14)
    -> filter::IPClassifier(src net 1.0.0.0/16,
                            src net 1.2.0.0/16,
                            -);
    filter[2]
    -> Discard;

    filter[0] // from the traffic generator
    -> FTMBInputLogger()
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip1, src)
    -> StoreIPAddress(10.70.0.8, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, src) // from aqua07
    -> StoreEtherAddress(f4:52:14:5a:90:70, dst) // to aqua08
    -> td1;

    filter[1] // from the master 1
//    -> IPPrint("from slave 1")
    -> FTMBOutputLogger(PER_PACKET 10)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip2, src)
    -> StoreIPAddress(10.70.0.9, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, src) // from aqua07
    -> StoreEtherAddress(e4:1d:2d:13:9c:60, dst) // to aqua09
//    -> IPPrint("To second")
    -> td2;
}

MBBlock(0, 1.1.1.1, 1.3.3.3);
