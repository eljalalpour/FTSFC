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
                            src net 1.4.0.0/16,
                            -);
    filter[2]
    -> Discard;

    filter[0]
    -> FTMBInputLogger()
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip1, src)
    -> StoreIPAddress(10.70.0.10, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9c:60, src) // from aqua09
    -> StoreEtherAddress(e4:1d:2d:0c:89:e0, dst) // to aqua10
    -> td1;

    filter[1]
    -> FTMBOutputLogger(PER_PACKET 10)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip2, src)
    -> StoreIPAddress(10.70.0.1, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9c:60, src) // from aqua09
    -> StoreEtherAddress(f4:52:14:69:a5:20, dst) // to aqua01
    -> td2;
}

MBBlock(0, 1.4.1.1, 1.6.6.6);