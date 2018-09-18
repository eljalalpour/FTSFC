// In aqua09
// aqua08 -> aqua09 -> aqua10

//
// Check list
// CHAIN size
// ID
// IPClassifier addresses
// StoreIPAddress dst ip
// StoreIPAddress src mac
// StoreIPAddress dst mac
//

shared_state::SharedLockFreeState(CHAIN 2, ID 1, F 1);

// stats_coll::StatsCollector(BUFFER 0,
//                           STATE 1,
//                           PATH ../../FTSFC/results/single-thread/replica2.csv,
//                           PERIOD 100)


FromDPDKDevice(0,0)
-> MarkIPHeader(14)
-> IPFilter(allow udp && src 1.2.0.0/16)
-> SetTimestamp
-> PMProcess
-> FTLockFreeCounter(0)
-> PMConstruct
-> ta::TimestampAccum
-> MarkIPHeader(14)
-> StoreIPAddress(1.3.1.1, src)
-> StoreIPAddress(192.168.1.110, dst)
-> StoreEtherAddress(0c:c4:7a:73:f9:ec, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:18, dst)
-> ToDPDKDevice(0);
