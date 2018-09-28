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

shared_state::SharedLockFreeState(CHAIN 5, ID 3, F 2);

// stats_coll::StatsCollector(BUFFER 0,
//                          STATE 1,
//                          PATH ../../FTSFC/results/single-thread/ch5-st1.csv,
//                          PERIOD 500)

elementclass FTBlock {
$index,$out |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.3.0.0/16)
//    -> IPPrint("From 3")
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct
    -> MarkIPHeader(14)
    -> StoreIPAddress($out, src)
    -> StoreIPAddress(10.70.0.11, dst)
    -> StoreEtherAddress(e4:1d:2d:0c:89:e0, src)
    -> StoreEtherAddress(e4:1d:2d:13:9e:e0, dst)
//    -> IPPrint("To 4")
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
-> b1::FTBlock(0,1.4.1.1)
-> td1;

// fd2
// -> b2::FTBlock(1,1.4.2.2)
// -> td2;
//
// fd3
// -> b3::FTBlock(2,1.4.3.3)
// -> td3;
//
// fd4
// -> b4::FTBlock(3,1.4.4.4)
// -> td4;
//
// fd5
// -> b5::FTBlock(4,1.4.5.5)
// -> td5;
//
// fd6
// -> b6::FTBlock(5,1.4.6.6)
// -> td6;
//
// fd7
// -> b7::FTBlock(6,1.4.7.7)
// -> td7;
//
// fd8
// -> b8::FTBlock(7,1.4.8.8)
// -> td8;
