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
-> StoreIPAddress(10.70.0.7, src)
-> StoreIPAddress(10.70.0.1, dst)
-> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
-> StoreEtherAddress(f4:52:14:69:a5:20, dst)
-> ToDPDKDevice(0,0);
