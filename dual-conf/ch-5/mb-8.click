// In aqua03
// aqua02 -> aqua03 -> aqua04

_state::SharedState;

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice($dev, 0);
    td1::ToDPDKDevice($dev, 0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.7.0.0/16)
    -> FTMBOutputLogger(PER_PACKET 3)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)   // from aqua 03
    -> StoreIPAddress(10.70.0.4, dst) // to aqua 04
    -> StoreEtherAddress(e4:1d:2d:13:9f:80, src) // from aqua 03
    -> StoreEtherAddress(e4:1d:2d:13:9f:e0, dst) // to aqua 04
    -> td1;
}

MBBlock(1, 1.8.1.1);