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

stats_coll:StatsCollector(BUFFER 0,
                          STATE 1,
                          PATH ../../FTSFC/results/single-thread/replica1.csv,
                          PERIOD 500)

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow udp && src 1.1.0.0/16)
-> SetTimestamp
-> PMProcess
-> PMConstruct
-> ta::TimestampAccum
-> MarkIPHeader(14)
-> StoreIPAddress(1.2.1.1, src)
-> StoreIPAddress(192.168.1.109, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
-> StoreEtherAddress(0c:c4:7a:73:f9:ec, dst)
-> ToDPDKDevice(0);
