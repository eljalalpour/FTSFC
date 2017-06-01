require(package "FTSFC");

FromDemp(vlan-tag-1.pcap)
->VLANDecap
->CheckIPHeader(14)
->NFCounterMB(ID 0)
->VLANEncap(VLAN_ID 6)
->VLANEncap(VLAN_ID 6)

->Queue

->FTFilterElement(6)
->CheckIPHeader(18)
->VLANDecap
->CheckIPHeader(14)
->NFCounterMB(ID 1)
->VLANEncap(VLAN_ID 7)
->VLANEncap(VLAN_ID 7)
->Queue
->ToDevice(p0);