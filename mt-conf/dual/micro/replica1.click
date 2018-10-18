// In aqua08
// aqua07 -> aqua08 -> aqua09

//
// Check list
// CHAIN size
// ID
// StoreIPAddress dst ip
// StoreIPAddress src mac
// StoreIPAddress dst mac
//

shared_state::SharedLockFreeState(CHAIN 2, ID 0, F 1);

stats_coll::StatsCollector(BUFFER 0,
                          STATE 1,
                          PATH ../FTSFC/results/single-thread/replica1.csv,
                          PERIOD 100)

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow udp && src 1.1.0.0/16)
-> SetTimestamp
-> PMProcess
-> FTLockFreeCounter(INDEX 0)
-> PMConstruct(ID $index)
-> ta::TimestampAccum
-> MarkIPHeader(14)
-> StoreIPAddress(1.2.1.1, src)
-> StoreIPAddress(10.70.0.9, dst)
-> StoreEtherAddress(f4:52:14:5a:90:70, src)
-> StoreEtherAddress(e4:1d:2d:13:9c:60, dst)
-> ToDPDKDevice(0);
