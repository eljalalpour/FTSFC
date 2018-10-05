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
-> StoreIPAddress(1.1.1.1, src)
-> StoreIPAddress(10.70.0.8, dst)
-> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
-> StoreEtherAddress(f4:52:14:5a:90:70, dst)
-> ToDPDKDevice(0)
