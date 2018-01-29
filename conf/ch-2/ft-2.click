AddressInfo(sender 192.168.233.7);

FromDPDKDevice(0)
//-> Print(2)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
//-> MarkIPHeader(14)
-> se::FTStateElement(ID 1, F 1);

se[0]
//-> MarkIPHeader(14)
-> Monitor(ID 1)
//-> MarkIPHeader(14)
-> [1]se;

se[1]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.8, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
//-> MarkIPHeader(14)
-> be::FTBufferElement;

be[0]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.6, dst)// set dst ip to receiver
-> StoreEtherAddress(0c:c4:7a:73:fa:46, dst)
//-> Print(ToDest)
-> ToDPDKDevice(0);

be[1]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.7, dst) // set dst ip to M1
-> StoreEtherAddress(0c:c4:7a:73:fa:54, dst)
//-> Print(ToFirst)
-> ToDPDKDevice(0);
