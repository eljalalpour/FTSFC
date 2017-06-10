require(package "FTSFC");

FTControlElement(10001);

FromDevice(p0)
->FTFilterElement(6)
->VLANDecap
->CheckIPHeader(14)
->se::FTStateElement(ID 2, VLAN_ID 6, F 1)
->CheckIPHeader(14)
//->MB1::CounterMB(ID 1)
->CheckIPHeader(14)
->[1]se;se[1]
->CheckIPHeader(14)
->VLANEncap(VLAN_ID 7)
->VLANEncap(VLAN_ID 7)
->Queue
->ToDevice(p0);
