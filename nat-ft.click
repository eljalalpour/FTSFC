require(package "FTSFC");

FTControlElement(2);

AddressInfo(sender 10.70.0.8);

FromDevice(p1p1)
-> CheckIPHeader(14)
-> IPFilter(allow src sender)
//-> Strip(14)
//-> ctr::Counter()
-> se::FTStateElement(ID 1, F 1)
-> CheckIPHeader(14)
-> lnat::nat(ID 1)
-> [1]se;

se[1]
-> CheckIPHeader(14)
-> StoreIPAddress(10.70.0.9, src)
-> be::FTBufferElement[0]
-> CheckIPHeader(14)
-> StoreIPAddress(10.70.0.6, dst)// set dst ip to receiver
-> StoreEtherAddress(e4:1d:2d:13:9c:60, src)
-> StoreEtherAddress(e4:1d:2d:13:9e:c0, dst)
//-> EtherEncap(0x0800, e4:1d:2d:13:9c:60, e4:1d:2d:13:9e:c0)
//-> pe::FTPassElement;
-> tmpq::Queue;

be[1]
-> CheckIPHeader(14)
-> StoreIPAddress(10.70.0.7, dst) // set dst ip to M1
-> StoreEtherAddress(e4:1d:2d:13:9c:60, src)
-> StoreEtherAddress(e4:1d:2d:13:9e:d0, dst)
//-> EtherEncap(0x0800, e4:1d:2d:13:9c:60, e4:1d:2d:13:9e:d0)
//-> [1]pe;
-> tmpq;

//pe
tmpq
//-> CheckIPHeader
//-> Queue
//-> StoreIPAddress(10.70.0.10, dst)// set dst ip to receiver
//-> EtherEncap(0x0800, e4:1d:2d:13:9c:60, e4:1d:2d:0c:89:e0)
//-> Print(out)
-> ToDevice(p1p1) 
