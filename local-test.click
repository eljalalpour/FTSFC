require(package "FTSFC");

// Source
FromDump(/Users/eghaznavi/Downloads/dumps/test/input-vlan.pcap, STOP true)

//Network
->begin::FTRandomDropElement(0.0)

// First Replica
->FTFilterElement(1, 4)
->CheckIPHeader(18)
->FTAppenderElement(1)
->VLANDecap
->CheckIPHeader(14)
->se1::FTStateElement(ID 1, VLAN_ID 1, F 1)
->CheckIPHeader(14)
->MB1::CounterMB(ID 1)
->CheckIPHeader(14)
->[1]se1;

se1[1]
->VLANEncap(VLAN_ID 2)

// Network
->FTRandomDropElement(0.3)

// Last replica
->FTFilterElement(2)
->VLANDecap
->CheckIPHeader(14)
->se2::FTStateElement(ID 2, VLAN_ID 2, F 1)
->CheckIPHeader(14)
->MB2::CounterMB(ID 2)
->CheckIPHeader(14)
->[1]se2;

se2[1]
->CheckIPHeader(14)
->be::FTBufferElement
->CheckIPHeader(14)
->pe::FTPassElement;

be[1]
->CheckIPHeader(14)
->[1]pe;

pe
//->VLANDecap
->VLANEncap(VLAN_ID 3)
->ToDump(/Users/eghaznavi/Downloads/dumps/test/outside-world-output.pcap);

pe[1]
//->VLANDecap
->VLANEncap(VLAN_ID 4)
//->ToDump(/Users/eghaznavi/Downloads/dumps/test/chain-begin-output.pcap)
->begin;