require(package "FTSFC");

FTControlElement(10001);

FromDevice(p1p1)
->ctr1::Counter()
//->FTFilterElement(7)
//->VLANDecap
->CheckIPHeader(14)
->ctr2::Counter()
//->se::FTStateElement(ID 2, VLAN_ID 7, F 1)
//->CheckIPHeader(14)
//->MB%d::CounterMB(ID 2)
//->Strip(14)
->my_ip :: IPClassifier(dst tcp ssh,
			dst tcp www or https,
			src tcp port ftp,
			tcp or udp,
			-);

nat1::natnf();
my_ip[0] -> nat1;
my_ip[1] -> nat1;
my_ip[2] -> nat1;
my_ip[3] -> nat1;
my_ip[4] -> Print(shit) -> Discard;

//->natnf()
nat1
->ctr3::Counter()
//->CheckIPHeader(14)
//->[1]se;

//se[1]
//->CheckIPHeader(14)
//->be::FTBufferElement
//->VLANEncap(VLAN_ID 8)
//->VLANEncap(VLAN_ID 8)
//->pe::FTPassElement;

//be[1]
//->VLANEncap(VLAN_ID 9)
//->VLANEncap(VLAN_ID 9)
//->[1]pe;

//pe
->Queue
->ctr::Counter()
->IPEncap(4, 10.70.0.9, 10.70.0.6)
->EtherEncap(0x0800, e4:1d:2d:13:9c:60, e4:1d:2d:13:9e:c0)
->ToDevice(p1p1);
