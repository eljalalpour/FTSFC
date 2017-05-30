require(package "FTSFC");
FromDevice(p0)
->FTFilterElement(6)
->CheckIPHeader(18)
->VLANDecap
->CheckIPHeader(14)
->NFCounterMB(ID 1)
->VLANEncap(VLAN_ID 7)
->VLANEncap(VLAN_ID 7)
->Queue
->ToDevice(p0);