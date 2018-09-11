// In aqua08
// aqua07 -> aqua08 -> aqua01

elementclass NFBlock {
$index,$src_ip |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 22.1.2.0/8)
    -> IPPrint($index)
//    -> NFAtomicCounter(INDEX $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(192.168.1.101, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:72, dst)
    -> output
}

FromDPDKDevice(0)
-> NFBlock(0,22.1.3.1)
-> ToDPDKDevice(0);
