fdA :: FromDPDKDevice(0);
//fdB :: FromDPDKDevice(0,1);
td :: ToDPDKDevice(0);

//StaticThreadSched(fdA 0,fdB 1);

fdA
-> MarkIPHeader(14)
//-> Print("A")
-> Counter
-> StoreIPAddress(10.70.0.6, dst)// set dst ip to receiver
-> StoreEtherAddress(e4:1d:2d:13:9e:c0, dst)
-> StoreIPAddress(10.70.0.7, src)
-> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
-> td;

//fdB
//-> MarkIPHeader(14)
//-> Print("B")
//-> StoreIPAddress(10.70.0.6, dst)// set dst ip to receiver
//-> StoreEtherAddress(e4:1d:2d:13:9e:c0, dst)
//-> StoreIPAddress(10.70.0.7, src)
//-> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
//-> td;
root@aqua01:/home/eghaznavi/Milad# cat click-modular-router/multi-thread-dpdk.click
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
-> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
-> tdA;

fdB
-> MarkIPHeader(14)
//-> Print("B")
-> Counter
-> StoreIPAddress(10.70.0.6, dst)// set dst ip to receiver
-> StoreEtherAddress(e4:1d:2d:13:9e:c0, dst)
-> StoreIPAddress(10.70.0.7, src)
-> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
-> tdB;
