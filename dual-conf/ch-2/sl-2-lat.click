// In aqua07
// aqua07 -> aqua09 -> aqua10
// aqua10 -> aqua09 -> aqua01


elementclass MBBlock {
$index,$src_ip1,$src_ip2 |
    fd1::FromDPDKDevice(1,0);
    td1::ToDPDKDevice(1,0);
    td2::ToDPDKDevice(1,1);

    StaticThreadSched(td1 0, td2 1);

    fd1
    -> MarkIPHeader(14)
    -> filter::IPClassifier(src net 1.3.0.0/16,
                            src net 1.5.0.0/16,
                            -);
    filter[2]
    -> Discard;

    filter[0] // from slave 1
//    -> IPPrint("From slave 1")
    -> FTMBInputLogger()
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip1, src)
    -> StoreIPAddress(192.168.1.110, dst)
    -> StoreEtherAddress(0c:c4:7a:73:f9:ec, src) // from aqua09
    -> StoreEtherAddress(0c:c4:7a:73:fa:18, dst) // to aqua10
    -> td1;

    filter[1] // from master 2
    -> FTMBOutputLogger(PER_PACKET 10)
//    -> IPPrint("from master 2")
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip2, src)
    -> StoreIPAddress(192.168.1.101, dst)
    -> StoreEtherAddress(0c:c4:7a:73:f9:ec, src) // from aqua09
    -> StoreEtherAddress(0c:c4:7a:73:fa:72, dst) // to aqua01
    -> td2;
}

MBBlock(0, 1.4.1.1, 1.6.6.6);
