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
-> buffer::Buffer(CHAIN 4);

// To the outside world
buffer[0]
-> MarkIPHeader(14)
-> ta::TimestampAccum
-> StoreIPAddress(1.4.1.1, src)
-> StoreIPAddress(10.70.0.1, dst)
-> StoreEtherAddress(e4:1d:2d:0c:89:e0, src)
-> StoreEtherAddress(f4:52:14:69:a5:20, dst)
-> ToDPDKDevice(0);

// To the forwarder
buffer[1] -> Truncate(380)
-> MarkIPHeader(14)
-> StoreIPAddress(2.0.1.1, src)
-> StoreIPAddress(10.70.0.7, dst)
-> StoreEtherAddress(e4:1d:2d:0c:89:e0, src)
-> StoreEtherAddress(e4:1d:2d:13:9e:d0, dst)
-> ToDPDKDevice(0);