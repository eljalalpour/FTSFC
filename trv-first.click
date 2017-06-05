require(package "FTSFC");

tr::TrivialCounterMB(ID 0, 10.12.4.10:22222);

FromDevice(p0)
->FTFilterElement(5)
->CheckIPHeader(18)
->VLANDecap
->CheckIPHeader(14)
->tr
->VLANEncap(VLAN_ID 6)
->VLANEncap(VLAN_ID 6)
->Queue
->ToDevice(p0);