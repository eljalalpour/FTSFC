AddressInfo(sender 10.70.0.8);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
//-> Print(Second)
//-> MarkIPHeader(14)
-> se::FTStateElement(ID 2, F 1);

se[0]
//-> MarkIPHeader(14)
-> Monitor(ID 2)
//-> MarkIPHeader(14)
-> [1]se;

se[1]
//-> MarkIPHeader(14)
-> StoreIPAddress(10.70.0.9, src)
-> StoreEtherAddress(e4:1d:2d:13:9c:60, src)
//-> MarkIPHeader(14)
-> be::FTBufferElement;

be[0]
-> MarkIPHeader(14)
-> StoreIPAddress(10.70.0.6, dst)// set dst ip to receiver
-> StoreEtherAddress(e4:1d:2d:13:9e:c0, dst)
//-> Print(ToDest)
-> ToDPDKDevice(0);

be[1]
-> MarkIPHeader(14)
-> StoreIPAddress(10.70.0.7, dst) // set dst ip to M1
-> StoreEtherAddress(e4:1d:2d:13:9e:d0, dst)
//-> Print(ToFirst)
-> ToDPDKDevice(0);
