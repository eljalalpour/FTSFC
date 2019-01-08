// In aqua10
// aqua09 -> aqua10 -> aqua11
//

//
// Check list
// CHAIN size
// ID
// IPClassifier addresses
// StoreIPAddress dst ip
// StoreIPAddress src mac
// StoreIPAddress dst mac
//

shared_state::SharedState(CHAIN 5, ID 3, F 2);

elementclass FTBlock {
$queue, $index, $out |
    fd1::FromDPDKDevice(1, $queue);
    td1::ToDPDKDevice(1, $queue);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.3.0.0/16)
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct(ID $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($out, src)
    -> StoreIPAddress(10.70.0.11, dst)
        -> StoreEtherAddress(e4:1d:2d:0c:89:e0, src)
        -> StoreEtherAddress(e4:1d:2d:13:9e:e0, dst)
//    -> IPPrint("To 3")
    -> td1;
}

FTBlock(0, 0, 1.4.1.1);
