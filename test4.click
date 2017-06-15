//require(package "FTSFC");

//FTControlElement(10001);

FromDevice(p1p1)
->CheckIPHeader(14)
->Strip(14)
//->ctr::Counter()
//->Print(ok)
->Queue 
->IPEncap(4, 172.19.18.110, 172.19.18.106) 
->EtherEncap(0x0800, 0c:c4:7a:73:f8:ec, e4:1d:2d:13:9e:c0)
->ctr::Counter()
->ToDevice(em1.3556);
