// In aqua08
// aqua07 -> aqua08 -> aqua09

_state::SharedState;

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice($dev, 0);
    td1::ToDPDKDevice($dev, 0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
    -> FTMBOutputLogger(PER_PACKET 3)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)   // from aqua 8
    -> StoreIPAddress(10.70.0.9, dst) // to aqua 9
    -> StoreEtherAddress(f4:52:14:5a:90:70, src) // from aqua 8
    -> StoreEtherAddress(e4:1d:2d:13:9c:60, dst) // to aqua 9
    -> td1;
}

MBBlock(1, 1.2.1.1);
