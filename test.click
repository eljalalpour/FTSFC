require(package "FTSFC");

FromDump(small-packets-1.pcap)
->VLANEncap(VLAN_ID 5)
->CheckIPHeader(18)
->FTAppenderElement(5)
->VLANDecap
->CheckIPHeader(14)
->se::FTStateElement(ID 1, VLAN_ID 4, F 1)
->CounterMB(ID 1)
->CheckIPHeader(14)
->[1]se;

se[1]
->Print(end)
->Discard;