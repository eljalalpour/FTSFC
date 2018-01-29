AddressInfo(sender 192.168.233.10);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
//-> Print(5)
//-> MarkIPHeader(14)
-> se::FTStateElement(ID 4, F 2);

se[0]
//-> MarkIPHeader(14)
-> Monitor(ID 4)
//-> MarkIPHeader(14)
-> [1]se;

se[1]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.11, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:e6, src)
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
//-> Print(ToChainBegin)
-> ToDPDKDevice(0);
