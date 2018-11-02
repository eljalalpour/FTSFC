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
    -> StoreIPAddress(192.168.1.108, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, src) // from aqua07
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst) // to aqua08
    -> td1;

    filter[1] // from the master 1
//    -> IPPrint("from slave 1")
    -> FTMBOutputLogger(PER_PACKET 10)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip2, src)
    -> StoreIPAddress(192.168.1.109, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, src) // from aqua07
    -> StoreEtherAddress(0c:c4:7a:73:f9:ec, dst) // to aqua09
//    -> IPPrint("To second")
    -> td2;
}

MBBlock(0, 1.1.1.1, 1.3.3.3);
