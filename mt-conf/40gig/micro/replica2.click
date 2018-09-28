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

stats_coll::StatsCollector(BUFFER 0,
                          STATE 1,
                          PATH ../FTSFC/results/single-thread/replica2.csv,
                          PERIOD 100)


FromDPDKDevice(0,0)
-> MarkIPHeader(14)
-> IPFilter(allow udp && src 1.2.0.0/16)
-> SetTimestamp
-> PMProcess
-> FTLockFreeCounter(INDEX 0)
-> PMConstruct
-> ta::TimestampAccum
-> MarkIPHeader(14)
-> StoreIPAddress(1.3.1.1, src)
-> StoreIPAddress(10.70.0.10, dst)
-> StoreEtherAddress(e4:1d:2d:13:9c:60, src)
-> StoreEtherAddress(e4:1d:2d:0c:89:e0, dst)
-> ToDPDKDevice(0);
