// In aqua10
// aqua09 -> aqua10 -> aqua01

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
    -> StoreIPAddress(192.168.1.101, dst) // to aqua 1
    -> StoreEtherAddress(0c:c4:7a:73:fa:18, src) // from aqua 10
    -> StoreEtherAddress(0c:c4:7a:73:fa:72, dst) // to aqua 1
    -> td1;
}

MBBlock(0, 1.4.1.1);