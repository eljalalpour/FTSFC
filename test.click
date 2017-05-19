require(package "FTSFC");

FromDump(small-packets-1.pcap, STOP true)
->Print
->Test(ID 1)
->Test(ID 2)
->Discard;