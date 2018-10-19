// In aqua07
// aqua01 -> aqua07 -> aqua08

shared_state::SharedLockFreeState(CHAIN 2, ID 0, F 1);

elementclass FTBlock {
$queue, $index, $src_ip |
    ld1::FromDPDKDevice(0, $queue);
    fd1::FromDPDKDevice(1, $queue);
    td1::ToDPDKDevice(1, $queue);
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
    -> StoreIPAddress(10.70.0.8, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
    -> StoreEtherAddress(f4:52:14:5a:90:70, dst)
//    -> IPPrint("To second")
    -> td1;
}

FTBlock(0, 0, 1.1.1.1);