require(package "FTSFC");

FromDevice(eth6)
->VLANEncap(VLAN_ID 4)
->CheckIPHeader(18)
->ap::FTAppenderElement(4)
->VLANDecap
->CheckIPHeader(14)
-> se::FTStateElement(ID 1, VLAN_ID 4, F 1)
//-> lnat::nat(ID 1);

-> Strip(14)
-> CheckIPHeader
-> filt::IPClassifier( tcp or udp,
		       -);
filt[1] -> Discard;
filt[0] -> lnat::nat(ID 1);

lnat 
//-> Print(ok) 
-> [1]se;

se[1]
-> Discard;
