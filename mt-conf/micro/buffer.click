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

// stats_coll::StatsCollector(BUFFER 1,
//                           STATE 0,
//                           PATH ../../FTSFC/results/single-thread/buffer.csv,
//                           PERIOD 100)


FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow udp && src 1.3.0.0/16)
-> SetTimestamp
-> buffer::Buffer(CHAIN 4, BATCH 1);

// To the outside world
buffer[0]
-> MarkIPHeader(14)
-> ta::TimestampAccum
-> StoreIPAddress(1.4.1.1, src)
-> StoreIPAddress(192.168.1.101, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:18, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:72, dst)
-> ToDPDKDevice(0);

// To the forwarder
buffer[1] -> Truncate(380)
-> MarkIPHeader(14)
-> StoreIPAddress(2.0.1.1, src)
-> StoreIPAddress(192.168.1.107, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:18, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:54, dst)
-> ToDPDKDevice(0);