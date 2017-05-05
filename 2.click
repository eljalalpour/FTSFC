require(package "FTSFC");

FromDevice(en0)
->SetVLANAnno
->FTFilterElement(15, 2, 3)
->ToDump(/Users/eghaznavi/Desktop/ttt.pcap);