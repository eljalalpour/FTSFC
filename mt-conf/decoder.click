elementclass DecoderBlock {
$index,$src_ip |
    input
//    -> IPPrint($index)
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 22.1.0.0/16)
    -> Decoder()
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(192.168.1.101, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:72, dst)
    -> output
}

FromDPDKDevice(0)
-> DecoderBlock(0,22.1.1.1)
-> ToDPDKDevice(0);
