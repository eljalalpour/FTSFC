AddressInfo(sender 10.70.0.6);
trans::Transmitter(10.0.7.12:10000);
counters::AtomicCounters;

elementclass CounterBlock {
$index |
    input
//    -> Print($index)
    -> MarkIPHeader(14)
    -> IPFilter(allow src sender)
    -> TFTAtomicCounter(INDEX $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress(10.70.0.7, src)
    -> StoreIPAddress(10.70.0.1, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
    -> StoreEtherAddress(e4:1d:2d:13:9e:c0, dst)
    -> output
}

FromDPDKDevice(0)
-> CounterBlock(0)
-> ToDPDKDevice(0);
