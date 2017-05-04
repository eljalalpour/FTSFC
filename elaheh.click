require(package "FTSFC");

FromDump(/Users/eghaznavi/Downloads/dumps/test.pcap)
->CheckIPHeader(14)
->ap::FTAppenderElement
->CheckIPHeader(14)
->se::FTStateElement(ID 1, FAILURE_COUNT 2, STATE )
->CheckIPHeader(14)
->cmb::CounterMB(ID 1);

cmb
->CheckIPHeader(14)
->[1]se;

se[1]
->CheckIPHeader(14)
->se2::FTStateElement(ID 2)
->CheckIPHeader(14)
->cmb2::CounterMB(ID 2);

cmb2
->CheckIPHeader(14)
->[1]se2;

se2[1]
->CheckIPHeader(14)
-> be :: FTBufferElement[1]
->CheckIPHeader(14)
->[1]ap;

be
->ToDump(/Users/eghaznavi/Desktop/naghi.pcap);

ControlSocket(TCP, 10000, VERBOSE true);