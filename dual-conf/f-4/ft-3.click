// In aqua09
// aqua08 -> aqua09 -> aqua10

shared_state::SharedLockFreeState(CHAIN 5, ID 2, F 4);

elementclass FTBlock {
$queue, $index, $out |
    fd1::FromDPDKDevice(1, $queue);
    td1::ToDPDKDevice(1, $queue);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.2.0.0/16)
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct(ID $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($out, src)
    -> StoreIPAddress(10.70.0.10, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9c:60, src)
    -> StoreEtherAddress(e4:1d:2d:0c:89:e0, dst)
//    -> IPPrint("To 3")
    -> td1;
}

FTBlock(0, 0, 1.3.1.1);