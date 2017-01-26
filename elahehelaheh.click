require(package "FTSFC");

//FromDevice(en0)

//FromDump(/Users/ejalalpo/Desktop/Test/ip.pcap, STOP true)
//-> CheckIPHeader(14)
//-> FTAppenderElement
//-> ToDump(/Users/ejalalpo/Desktop/Test/test-1.pcap);

//IPPrint(Packet, CONTENTS ASCII, ID true, LENGTH true)->Discard


//FromDump(/Users/ejalalpo/Desktop/Test/frame.cap_len.pcap, STOP true)
//->CheckIPHeader(14)
//ap::FTAppenderElement
//->CheckIPHeader(14)
//se[1]->ToDump(/Users/ejalalpo/Desktop/Test/test-1.pcap);








FromDevice(en0)
->MarkIPHeader(14)
->se::FTStateElement
->MarkIPHeader(14)
->cmb::CounterMB;

cmb
->MarkIPHeader(14)
->[1]se;

se[1]
->MarkIPHeader(14)
->fb::FTBufferElement[1]
->Queue
->ToDevice(en0);

fb
->ToDump(/Users/ejalalpo/Desktop/Test/toOutSideWorld.pcap);