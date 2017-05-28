require(package "FTSFC");

FromDevice(p0)
->FTFilterElement(6)
->VLANDecap
->CheckIPHeader(14)
->NFCounterMB(ID 6)
->VLANEncap(VLAN_ID 7)
->VLANEncap(VLAN_ID 7)
->Queue
->ToDevice(p0);