require(package "FTSFC");

FTControlElement(10001);
trans::Transmitter(ID 0, 10.10.0.1:121, 10.10.2.1:141)


//FromDevice(p0)
FromDump(small-packets-1.pcap)
->FTFilterElement(7)
->VLANDecap
->CheckIPHeader(14)
->se::FTStateElement(ID 2, VLAN_ID 7, F 1)
->CheckIPHeader(14)
->natft(ID 1)


->CheckIPHeader(14)
->[1]se;se[1]
->CheckIPHeader(14)
->be::FTBufferElement
->VLANEncap(VLAN_ID 8)
->VLANEncap(VLAN_ID 8)
->pe::FTPassElement;

be[1]
->VLANEncap(VLAN_ID 9)
->VLANEncap(VLAN_ID 9)
->[1]pe;pe
->Queue
->Print(ok)
->ToDevice(p0);
