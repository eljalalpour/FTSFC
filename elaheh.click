require(package "FTSFC");

//FromDevice(en0)

//FromDump(/Users/ejalalpo/Desktop/Test/ip.pcap, STOP true)
//-> CheckIPHeader(14)
//-> FTAppenderElement
//-> ToDump(/Users/ejalalpo/Desktop/Test/test-1.pcap);

//IPPrint(Packet, CONTENTS ASCII, ID true, LENGTH true)->Discard


FromDump(/Users/ejalalpo/Desktop/Test/frame.cap_len.pcap, STOP true)
->CheckIPHeader(14)
->ap::FTAppenderElement
->CheckIPHeader(14)
->se::FTStateElement
->CheckIPHeader(14)
->CounterMB
->CheckIPHeader(14)
->[1]se;
//se[1]->ToDump(/Users/ejalalpo/Desktop/Test/test-1.pcap);

se[1]
->CheckIPHeader(14)
->fb::FTBufferElement[1]
->ToDevice(en0);

FromDevice(en0)
->CheckIPHeader
->[1]ap;

fb[0]
->ToDump(/Users/ejalalpo/Desktop/Test/toOutSideWorld.pcap);