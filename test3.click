//require(package "FTSFC");

//FTControlElement(10001);

FromDevice(p1p1)
->CheckIPHeader(14)
->ctr::Counter()
//->Print(ok)
->Queue 
->IPEncap(4, 10.70.0.9, 10.70.0.6) 
->EtherEncap(0x0800, e4:1d:2d:13:9c:60, e4:1d:2d:13:9e:c0)
->ToDevice(p1p1);
