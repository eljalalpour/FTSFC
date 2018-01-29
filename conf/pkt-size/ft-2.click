AddressInfo(sender 10.70.0.7);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
//-> Print(First)
//-> MarkIPHeader(14)
-> se::FTStateElement(ID 1, F 1)
//-> MarkIPHeader(14)
-> Monitor(ID 1)
//-> MarkIPHeader(14)
-> [1]se;

se[1]
-> MarkIPHeader(14)
-> StoreIPAddress(10.70.0.8, src)
-> StoreIPAddress(10.70.0.9, dst)
-> StoreEtherAddress(f4:52:14:5a:90:70, src)
-> StoreEtherAddress(e4:1d:2d:13:9c:60, dst)
//-> Print(ToThird)
-> ToDPDKDevice(0);
