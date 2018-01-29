//require(package "FTSFC");

AddressInfo(sender 10.70.0.7);

FromDPDKDevice(0)
//FromDevice(p1p1)
-> CheckIPHeader(14)
-> IPFilter(allow src sender)
-> se::FTStateElement(ID 1, F 1)
-> MarkIPHeader(14)
-> CounterMB(ID 1)
-> MarkIPHeader(14)
-> [1]se;

se[1]
//-> Queue
-> MarkIPHeader(14)
-> StoreIPAddress(10.70.0.8, src)
-> StoreIPAddress(10.70.0.9, dst)
-> StoreEtherAddress(f4:52:14:5a:90:70, src)
-> StoreEtherAddress(e4:1d:2d:13:9c:60, dst)
//-> Queue
//-> ToDevice(p1p1)
-> ToDPDKDevice(0);
