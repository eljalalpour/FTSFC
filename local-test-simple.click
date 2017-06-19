require(package "FTSFC");

// Source
//FromDump(small-packets-1.pcap)

FTFastTCPGen(10, 500000, 60, 68:5b:35:c5:e9:39, 192.168.200.18, 68:5b:35:c5:e9:55, 192.168.200.17, 100, 10)

// First Replica
->begin::FTFilterElement(1, 4)
->CheckIPHeader(14)
->FTAppenderElement()
->CheckIPHeader(14)
->se1::FTStateElement(ID 1, F 1)
->CheckIPHeader(14)
->MB1::CounterMB(ID 1)
->CheckIPHeader(14)
->[1]se1;

se1[1]

// Network
->FTRandomDropElement(0.0)

// Last replica
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

//Network
->FTRandomDropElement(0.3)
->begin
