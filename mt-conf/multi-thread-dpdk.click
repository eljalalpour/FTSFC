fdA :: FromDPDKDevice(0,0);
fdB :: FromDPDKDevice(0,1);
tdA :: ToDPDKDevice(0,0);
tdB :: ToDPDKDevice(0,1);

StaticThreadSched(fdA 0,fdB 1);

fdA
-> MarkIPHeader(14)
//-> Print("A")
-> Counter
-> StoreIPAddress(10.70.0.6, dst)// set dst ip to receiver
-> StoreEtherAddress(e4:1d:2d:13:9e:c0, dst)
-> StoreIPAddress(10.70.0.7, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
-> tdA;

fdB
-> MarkIPHeader(14)
//-> Print("B")
-> Counter
-> StoreIPAddress(10.70.0.6, dst)// set dst ip to receiver
-> StoreEtherAddress(e4:1d:2d:13:9e:c0, dst)
-> StoreIPAddress(10.70.0.7, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
-> tdB;
