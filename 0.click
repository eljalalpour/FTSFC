require(package "FTSFC");

FromDump(/Users/eghaznavi/Downloads/dumps/test-2.pcap, STOP true)
->SetVLANAnno
->FTFilterElement(VLAN_ID 15)
->ToDump(/Users/eghaznavi/Downloads/dumps/test-3.pcap);