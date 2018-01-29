AddressInfo(sender 192.168.233.9);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
//-> Print(4)
//-> MarkIPHeader(14)
-> se::FTStateElement(ID 3, F 2)
//-> MarkIPHeader(14)
-> Monitor(ID 3)
//-> MarkIPHeader(14)
-> [1]se;

se[1]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.10, src)
-> StoreIPAddress(192.168.233.11, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:18, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:e6, dst)
//-> Print(REC)
-> ToDPDKDevice(0);
