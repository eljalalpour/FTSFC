AddressInfo(sender 192.168.1.107);

elementclass DecoderBlock {
$index |
    input
//    -> IPPrint($index)
    -> MarkIPHeader(14)
    -> IPFilter(allow src sender)
    -> Decoder()
    -> MarkIPHeader(14)
    -> StoreIPAddress(192.168.1.108, src)
    -> StoreIPAddress(192.168.1.101, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:72, dst)
    -> output
}

FromDPDKDevice(0)
-> DecoderBlock(0)
-> ToDPDKDevice(0);
