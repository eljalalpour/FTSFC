require(package "FTSFC");

tr::TrivialCounterMB(ID 1, 127.0.0.1:11111, 127.0.0.1:22222);

FromDump(small-packets-1.pcap)
->tr
->Discard;