require(package "FTSFC");

FromDump(/Users/eghaznavi/Downloads/dumps/net-2009-12-11-12-00.dmp, STOP true)
->CheckIPHeader(14)
->ap::FTAppenderElement
->CheckIPHeader(14)
->se::FTStateElement(ID 1)
->CheckIPHeader(14)
->CounterMB
->CheckIPHeader(14)
->[1]se;

FromDevice(en0)
->CheckIPHeader(14)
->[1]ap;

se[1]
->CheckIPHeader(14)
->se2::FTStateElement(ID 2)
->CheckIPHeader(14)
->CounterMB(ID 2)
->CheckIPHeader(14)
->[1]se2;

se2[1]
->Queue(1000000)
->CheckIPHeader(14)
->be2::FTBufferElement[1]
->Queue
->ToDump(/Users/eghaznavi/Downloads/dumps/toMilad.pcap);

be2
->ToDump(/Users/eghaznavi/Downloads/dumps/toOutSideWorld.pcap);