require(package "FTSFC");

FromDevice(%s)
->SetVLANAnno
->FTFilterElement(%d, %d)
->CheckIPHeader(14)
->se::FTStateElement(ID %d, VLAN_ID %d, F %d)
->CheckIPHeader(14)
->%s
->[1]se;

se[1]
->VLANEncap(VLAN_ID %d)
->ToDevice(%s);