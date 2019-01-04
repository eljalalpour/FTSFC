// In aqua08
// aqua07 -> aqua08 -> aqua09

_array::SharedArray;

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice($dev, 0);
    td1::ToDPDKDevice($dev, 0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
    -> FTMBOutputLogger(PER_PACKET 10)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(192.168.1.109, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, src) // from aqua08
    -> StoreEtherAddress(0c:c4:7a:73:f9:ec, dst) // to aqua09
    -> td1;
}

MBBlock(0, 1.2.2.2);
