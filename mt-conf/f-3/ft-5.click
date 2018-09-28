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


shared_state::SharedLockFreeState(CHAIN 5, ID 4, F 3);

// stats_coll::StatsCollector(BUFFER 0,
//                          STATE 1,
//                          PATH ../../FTSFC/results/single-thread/ch5-st1.csv,
//                          PERIOD 500)

elementclass FTBlock {
$index,$out,$forwarder |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.4.0.0/16)
//    -> IPPrint("From 4")
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct
    -> buffer::Buffer(CHAIN 5);

    // To the outside world
    buffer[0]
    -> MarkIPHeader(14)
    -> StoreIPAddress($out, src)
    -> StoreIPAddress(192.168.1.101, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:e6, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:72, dst)
//    -> Print("For latency", 300)
//    -> IPPrint("To 0")
    -> output;

    // To the forwarder
    buffer[1]
//    -> Truncate(380)
    -> MarkIPHeader(14)
    -> StoreIPAddress($forwarder, src)
    -> StoreIPAddress(192.168.1.107, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:e6, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, dst)
//    -> IPPrint("To 1")
    -> output;
}

fd1::FromDPDKDevice(0,0);
// fd2::FromDPDKDevice(0,1);
// fd3::FromDPDKDevice(0,2);
// fd4::FromDPDKDevice(0,3);
// fd5::FromDPDKDevice(0,4);
// fd6::FromDPDKDevice(0,5);
// fd7::FromDPDKDevice(0,6);
// fd8::FromDPDKDevice(0,7);

td1::ToDPDKDevice(0,0);
// td2::ToDPDKDevice(0,1);
// td3::ToDPDKDevice(0,2);
// td4::ToDPDKDevice(0,3);
// td5::ToDPDKDevice(0,4);
// td6::ToDPDKDevice(0,5);
// td7::ToDPDKDevice(0,6);
// td8::ToDPDKDevice(0,7);

// StaticThreadSched(fd1 0, fd2 1);
// StaticThreadSched(fd1 0, fd2 1, fd3 2)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4, fd6 5)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4, fd6 5, fd7 6)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4, fd6 5, fd7 6, fd8 7)

fd1
-> b1::FTBlock(0,1.5.1.1,2.0.1.1)
-> td1;

// fd2
// -> b2::FTBlock(1,1.5.2.2,2.0.2.2)
// -> td2;
//
// fd3
// -> b3::FTBlock(2,1.5.3.3,2.0.3.3)
// -> td3;
//
// fd4
// -> b4::FTBlock(3,1.5.4.4,2.0.4.4)
// -> td4;
//
// fd5
// -> b5::FTBlock(4,1.5.5.5,2.0.5.5)
// -> td5;
//
// fd6
// -> b6::FTBlock(5,1.5.6.6,2.0.6.6)
// -> td6;
//
// fd7
// -> b7::FTBlock(6,1.5.7.7,2.0.7.7)
// -> td7;
//
// fd8
// -> b8::FTBlock(7,1.5.8.8,2.0.8.8)
// -> td8;
