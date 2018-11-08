// In aqua10
// aqua09 -> aqua10 -> aqua11

array::LockFreeArray;

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice($dev, 0);
    td1::ToDPDKDevice($dev, 0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.3.0.0/16)
    -> FTMBOutputLogger(PER_PACKET 3)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)    // from aqua 10
    -> StoreIPAddress(10.70.0.11, dst) // to aqua 11
    -> StoreEtherAddress(e4:1d:2d:0c:89:e0, src) // from aqua 10
    -> StoreEtherAddress(e4:1d:2d:13:9e:e0, dst) // to aqua 11
    -> td1;
}

MBBlock(0, 1.4.1.1);