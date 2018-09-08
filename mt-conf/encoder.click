elementclass EncoderBlock {
$index |
    input
//    -> Print($index)
    -> MarkIPHeader(14)
    -> IPFilter(allow src sender)
    -> Decoder()
    -> MarkIPHeader(14)
    -> StoreIPAddress(192.168.1.107, src)
    -> StoreIPAddress(192.168.1.108, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst)
    -> output
}

FromDPDKDevice(0)
-> EncoderBlock(0)
-> ToDPDKDevice(0);
