require(package "FTSFC");
FTControlElement(10001);
//FromDump(small-packets-1.pcap)
FromDevice(en0)
->VLANEncap(VLAN_ID 5)
->FTFilterElement(5, 8)
->CheckIPHeader(18)
->FTAppenderElement(5)
->VLANDecap
//->Queue
->CheckIPHeader(14)
->se::FTStateElement(ID 0, VLAN_ID 5, F 1)
->CheckIPHeader(18)
->MB0::CounterMB(ID 0)
->[1]se;

se[1]
->Queue
->ToDevice(en0);