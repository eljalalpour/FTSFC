require(package "FTSFC");

FTControlElement(10001);

FromDevice(p0)
->FTFilterElement(7)
->VLANDecap
->CheckIPHeader(14)
->se::FTStateElement(ID 2, VLAN_ID 7, F 1)
->CheckIPHeader(14)
//->MB1::CounterMB(ID 1)
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
->ToDevice(p0);
