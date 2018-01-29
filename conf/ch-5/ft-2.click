AddressInfo(sender 192.168.233.7);

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
-> StoreIPAddress(192.168.233.8, src)
-> StoreIPAddress(192.168.233.9, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
-> StoreEtherAddress(0c:c4:7a:73:f9:ec, dst)
//-> Print(ToThird)
-> ToDPDKDevice(0);
