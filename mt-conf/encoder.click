elementclass EncoderBlock {
$index,$src_ip |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 22.1.0.0/16)
//    -> IPPrint($index)
    -> Encoder()
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(192.168.1.108, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst)
    -> output
}

FromDPDKDevice(0)
-> EncoderBlock(0,22.1.1.1)
-> ToDPDKDevice(0);
