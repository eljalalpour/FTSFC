//require(package "FTSFC");

//FTControlElement(10001);

FromDevice(p1p1)
->CheckIPHeader(14)
->ctr::Counter()
//->Print(ok)
->Queue 
->IPEncap(4, 10.70.0.8, 10.70.0.9) 
->EtherEncap(0x0800, f4:52:14:5a:90:70, e4:1d:2d:13:9c:60)
->ToDevice(p1p1);
