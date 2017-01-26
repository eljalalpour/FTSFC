require(package "FTSFC");

FromDump(/Users/eghaznavi/Downloads/dumps/net-2009-12-11-12-00.dmp)
->CheckIPHeader(14)
->ap::FTAppenderElement
->CheckIPHeader(14)
->se::FTStateElement
->CheckIPHeader(14)
->CounterMB
->CheckIPHeader(14)
->[1]se;

se[1]
->Queue(1000000)
->ToDevice(en0);

FromDevice(en0)
//FromDump(/Users/eghaznavi/Downloads/dumps/Hassantabar.pcap)
->CheckIPHeader(14)
->[1]ap;