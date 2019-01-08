// In aqua08
// aqua07 -> aqua08 -> aqua09

shared_state::SharedState(CHAIN 5, ID 1, F 4);

elementclass FTBlock {
$queue, $index, $out |
    fd1::FromDPDKDevice(1, $queue);
    td1::ToDPDKDevice(1, $queue);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct(ID $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($out, src)
    -> StoreIPAddress(10.70.0.9, dst)
    -> StoreEtherAddress(f4:52:14:5a:90:70, src)
    -> StoreEtherAddress(e4:1d:2d:13:9c:60, dst)
//    -> IPPrint("To 3")
    -> td1;
}

FTBlock(0, 0, 1.2.1.1)
