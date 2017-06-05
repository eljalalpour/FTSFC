require(package "FTSFC");

tr::TrivialCounterMB(ID 0, 192.168.200.12:22222);

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