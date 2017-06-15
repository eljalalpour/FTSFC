require(package "FTSFC");

//FromDump(small-packets-1.pcap)
FromDevice(eth6)
->Print(ok)
->CheckIPHeader(14)
->SetVLANAnno
->Queue
->FTFilterElement(1,4)
->ap::FTAppenderElement(1)
->CheckIPHeader(14)
->se1::FTStateElement(ID 1, F 1)
->CheckIPHeader(14)
->cmb::Monitor(ID 1);

cmb
-> [1]se1
-> Discard;

se1[1]
-> Discard;
