require(package "FTSFC");

tr::TrivialCounterMB(ID 1, 192.168.200.12:22222);

FromDevice(p0)
->FTFilterElement(6)
->VLANDecap
->CheckIPHeader(14)
->tr
->VLANEncap(VLAN_ID 7)
->VLANEncap(VLAN_ID 7)
->Queue
->ToDevice(p0);