require(package "FTSFC");



FromDump(vlan-tag-1.pcap)

//FromDevice(eth6)

//->Print(ok)

->VLANDecap

->CheckIPHeader(14)

//->SetVLANAnno

->Queue

//->FTFilterElement(1,4)

->ap::FTAppenderElement(1)

->CheckIPHeader(14)

->se1::FTStateElement(ID 1, F 1)

->Print(ok)

->CheckIPHeader(14)

->cmb::Print(ok)

//->cmb::Monitor(ID 1);

//->Print(ok)

//->cmb::CounterBU();



cmb

-> [1]se1

-> Discard;



se1[1]

-> Discard;