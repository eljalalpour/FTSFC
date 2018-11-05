// In aqua10
// aqua09 -> aqua10 -> aqua01
//                  -> aqua07

//
// Check list
// CHAIN size
// ID
// IPClassifier addresses
// StoreIPAddress dst ip
// StoreIPAddress src mac
// StoreIPAddress dst mac
//


shared_state::SharedLockFreeState(CHAIN 4, ID 3, F 1);

elementclass FTBlock {
$queue, $index, $out, $forwarder |
    fd1::FromDPDKDevice(1,$queue);
    td1::ToDPDKDevice(1,$queue);
    bd1::ToDPDKDevice(0,$queue);
    buffer::Buffer(BATCH 1, CHAIN 4);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.3.0.0/16)
//    -> IPPrint("From 3")
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct(ID $index)
    -> buffer;

    // To the outside world
    buffer[0]
    -> MarkIPHeader(14)
    -> StoreIPAddress($out, src)
    -> StoreIPAddress(10.70.0.1, dst)
    -> StoreEtherAddress(e4:1d:2d:0c:89:e0, src)
    -> StoreEtherAddress(f4:52:14:69:a5:20, dst)
//    -> IPPrint("To outside world")
    -> td1;

    // To the forwarder
    buffer[1]
    -> MarkIPHeader(14)
    -> StoreIPAddress($forwarder, src)
    -> StoreIPAddress(192.168.1.107, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:18, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, dst)
//    -> IPPrint("To the first")
    -> bd1;
}

FTBlock(0, 0, 1.4.1.1, 2.0.1.1);
