require(package "FTSFC");

FromDevice(p0)
->FTFilterElement(5)
->CheckIPHeader(18)
->VLANDecap
->CheckIPHeader(14)
->NFCounterMB(ID 0)
->VLANEncap(VLAN_ID 6)
->VLANEncap(VLAN_ID 6)
->Queue
->ToDevice(p0);