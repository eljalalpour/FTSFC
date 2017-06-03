require(package "FTSFC");

FromDump(small-packets-1.pcap)
->CheckIPHeader(14)
->NFCounterMB(ID 0)
->CheckIPHeader(14)
->NFCounterMB(ID 1)
->Discard;