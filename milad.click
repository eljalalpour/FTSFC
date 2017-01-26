require(package "FTSFC");

FromDump(/Users/eghaznavi/Downloads/dumps/net-2009-11-23-09-13.dmp)
->CheckIPHeader(14)
->ap::FTAppenderElement
->CheckIPHeader(14)
->se::FTStateElement
->CheckIPHeader(14)
->CounterMB
->CheckIPHeader(14)
->[1]se;

se[1]
->Queue
->ToDevice(en0);

FromDevice(en0)
->IPPrint(Packet, PAYLOAD ASCII, LENGTH 1)
->CheckIPHeader(14)
->[1]ap;