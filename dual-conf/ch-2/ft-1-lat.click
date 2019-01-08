// In aqua07
// aqua01 -> aqua07 -> aqua08

shared_state::SharedState(CHAIN 2, ID 0, F 1);

elementclass FTBlock {
$queue, $index, $src_ip |
    ld1::FromDPDKDevice(1, $queue);
    fd1::FromDPDKDevice(0, $queue);
    td1::ToDPDKDevice(0, $queue);
    forwarder::Forwarder(CHAIN 2);

    ld1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 2.0.0.0/16)
//    -> IPPrint("From the buffer")
    -> [1]forwarder;

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.0.0.0/16)
//    -> IPPrint("From the source")
    -> forwarder
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct(ID $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(192.168.1.108, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst)
//    -> IPPrint("To second")
    -> td1;
}

FTBlock(0, 0, 1.1.1.1);
