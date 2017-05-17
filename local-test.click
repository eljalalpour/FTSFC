require(package "FTSFC");

// Source
FromDump(/Users/eghaznavi/Downloads/dumps/test/input.pcap, STOP true)
->begin::Queue
->SetVLANAnno
->VLANEncap(VLAN_ID 1)

// First Replica
->SetVLANAnno
//->Queue
->FTFilterElement(1)
->CheckIPHeader(14)
->se1::FTStateElement(ID 1, VLAN_ID 1, F 1)
->CheckIPHeader(14)
->MB1::CounterMB
->[1]se1;
se1[1]
->SetVLANAnno
->VLANEncap(VLAN_ID 2)

// Last replica
->FTFilterElement(2)
->CheckIPHeader(14)
->se2::FTStateElement(ID 2, VLAN_ID 2, F 1)
->CheckIPHeader(14)
->MB2::CounterMB
->[1]se2;

se2[1]
->be::FTBufferElement
//->Queue
->SetVLANAnno
->VLANEncap(VLAN_ID 3)
->pe::FTPassElement;

be[1]
//->Queue
->SetVLANAnno
->VLANEncap(VLAN_ID 4)
->[1]pe;

pe
->ToDump(/Users/eghaznavi/Downloads/dumps/test/output.pcap);

pe[1]
->begin;