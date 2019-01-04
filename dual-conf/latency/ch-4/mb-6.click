// In aqua12
// aqua11 -> aqua12 -> aqua02

_array::SharedArray;

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice($dev, 0);
    td1::ToDPDKDevice($dev, 0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.5.0.0/16)
    -> FTMBOutputLogger(PER_PACKET 3)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)    // from aqua 12
    -> StoreIPAddress(10.70.0.2, dst) // to aqua 02
    -> StoreEtherAddress(0c:c4:7a:73:fa:9e, src) // from aqua 12
    -> StoreEtherAddress(0c:c4:7a:73:fa:dc, dst) // to aqua 02
    -> td1;
}

MBBlock(0, 1.6.1.1);