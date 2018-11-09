// In aqua05
// aqua04 -> aqua05 -> aqua01

array::LockFreeArray;

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice($dev, 0);
    td1::ToDPDKDevice($dev, 0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.9.0.0/16)
    -> FTMBOutputLogger(PER_PACKET 3)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)   // from aqua 05
    -> StoreIPAddress(192.168.1.101, dst) // to aqua 01
    -> StoreEtherAddress(0c:c4:7a:73:fa:6e, src) // from aqua 05
    -> StoreEtherAddress(0c:c4:7a:73:fa:72, dst) // to aqua 01
    -> td1;
}

MBBlock(1, 1.10.1.1);