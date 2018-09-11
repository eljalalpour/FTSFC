// In aqua07
// aqua01 -> aqua07 -> aqua08

elementclass NFBlock {
$index,$src_ip |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 22.1.1.0/8)
    -> IPPrint($index)
//    -> NFAtomicCounter(INDEX $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(192.168.1.108, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst)
    -> output
}

FromDPDKDevice(0)
-> NFBlock(0,22.1.2.1)
-> ToDPDKDevice(0);
