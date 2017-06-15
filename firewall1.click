FromDevice(p0)
->FTFilterElement(5, 9)
->CheckIPHeader(18)
->FTAppenderElement(5)
->VLANDecap
->CheckIPHeader(14)
->se::FTStateElement(ID 0, VLAN_ID 5, F 1)
->CheckIPHeader(14)
//->MB0::CounterMB(ID 0)
->CheckIPHeader(14)
->[1]se;se[1]
->VLANEncap(VLAN_ID 6)
->VLANEncap(VLAN_ID 6)
->Queue
->ToDevice(p0)
