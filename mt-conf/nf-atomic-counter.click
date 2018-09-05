AddressInfo(sender 10.70.0.1);

elementclass CounterBlock {
$index |
    input
    -> IPFilter(allow src sender)
    -> NFAtomicCounter(INDEX $index)
    -> StoreIPAddress(10.70.0.7, src)
    -> StoreIPAddress(10.70.0.1, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
    -> StoreEtherAddress(f4:52:14:69:a5:20, dst)
    -> output
}

FromDPDKDevice(0)
-> CounterBlock(0)
-> ToDPDKDevice(0);