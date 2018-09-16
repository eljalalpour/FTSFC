// In aqua07
// aqua01 -> aqua07 -> aqua01

FromDPDKDevice(0,0)
-> MarkIPHeader(14)
-> filter::IPClassifier(src net 1.0.0.0/16,
                        src net 2.0.0.0/16,
                        -);
filter[1]
-> Discard;

filter[2]
-> Discard;

filter[0]
-> MarkIPHeader(14)
-> SetTimestamp
-> StoreIPAddress(192.168.1.107, src)
-> StoreIPAddress(192.168.1.101, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:72, dst)
-> ta::TimestampAccum
-> ToDPDKDevice(0,0);
