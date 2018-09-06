AddressInfo(sender 192.168.1.101);
array::LockFreeArray;

elementclass CounterBlock {
$index |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow src sender)
//    -> IPPrint($index)
    -> NFLockFreeCounter(INDEX $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress(192.168.1.107, src)
    -> StoreIPAddress(192.168.1.101, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:72, dst)
    -> output
}

FromDPDKDevice(0)
-> CounterBlock(0)
-> ToDPDKDevice(0)
