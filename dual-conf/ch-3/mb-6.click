// In aqua12
// aqua11 -> aqua12 -> aqua01

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
    -> StoreIPAddress(10.70.0.1, dst) // to aqua 01
    -> StoreEtherAddress(e4:1d:2d:0c:89:d0, src) // from aqua 12
    -> StoreEtherAddress(f4:52:14:69:a5:20, dst) // to aqua 01
    -> td1;
}

MBBlock(1, 1.6.1.1);