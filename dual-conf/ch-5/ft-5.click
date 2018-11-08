// In aqua11
// aqua10 -> aqua11 -> aqua01
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


shared_state::SharedLockFreeState(CHAIN 5, ID 4, F 1);

// stats_coll::StatsCollector(BUFFER 0,
//                          STATE 1,
//                          PATH ../../FTSFC/results/single-thread/ch5-st1.csv,
//                          PERIOD 500)

elementclass FTBlock {
$queue, $index, $out, $forwarder |
    fd1::FromDPDKDevice(1,$queue);
    td1::ToDPDKDevice(1,$queue);
    bd1::ToDPDKDevice(0,$queue);
    buffer::Buffer(BATCH 5, CHAIN 5);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.4.0.0/16)
//    -> IPPrint("From 4")
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct(ID $index)
    -> buffer;

    // To the outside world
    buffer[0]
    -> MarkIPHeader(14)
    -> StoreIPAddress($out, src)
    -> StoreIPAddress(10.70.0.1, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9e:e0, src)
    -> StoreEtherAddress(f4:52:14:69:a5:20, dst)
//    -> Print("For latency", 300)
//    -> IPPrint("To 0")
    -> td1;

    // To the forwarder
    buffer[1]
    -> MarkIPHeader(14)
    -> StoreIPAddress($forwarder, src)
    -> StoreIPAddress(192.168.1.107, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:e6, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, dst)
//    -> IPPrint("To the first")
    -> bd1;
}

FTBlock(0, 0, 1.5.1.1, 2.0.1.1);