// In aqua07
// aqua01 -> aqua07 -> aqua08

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> filter::IPClassifier(src net 1.0.0.0/16,
                        src net 2.0.0.0/16,
                        -);

filter[2]
-> Discard;

filter[0]
-> SetTimestamp
-> [0]forwarder::Forwarder(CHAIN 2);

filter[1]
-> [1]forwarder;

forwarder[0]
-> ta::TimestampAccum
-> MarkIPHeader(14)
-> StoreIPAddress($src_ip, src)
-> StoreIPAddress(192.168.1.108, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst)
-> ToDPDKDevice(0)
