AddressInfo(sender 192.168.233.8);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
//-> Print(3)
//-> MarkIPHeader(14)
-> se::FTStateElement(ID 2, F 4)
//-> MarkIPHeader(14)
-> Monitor(ID 2)
//-> MarkIPHeader(14)
-> [1]se;

se[1]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.9, src)
-> StoreIPAddress(192.168.233.10, dst)
-> StoreEtherAddress(0c:c4:7a:73:f9:ec, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:18, dst)
//-> Print(REC)
-> ToDPDKDevice(0);
