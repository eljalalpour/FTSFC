elementclass FTCounterBlock {
$index |
    input
//    -> IPPrint($index)
    -> MarkIPHeader(14)
    -> fil::IPClassifier(proto udp,
    src host 10.1.,
                        src host 192.168.233.8,
                        -);
    -> Encoder()
    -> MarkIPHeader(14)
    -> StoreIPAddress(192.168.1.107, src)
    -> StoreIPAddress(192.168.1.108, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst)
    -> output
}

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> fil::IPClassifier(src host 192.168.233.6,
                    src host 192.168.233.8,
                    -);

FromDPDKDevice(0)
-> EncoderBlock(0)
-> ToDPDKDevice(0);
