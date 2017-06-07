require(package "FTSFC");

tr::TrivialCounterMB(ID 1, 127.0.0.1:11111, 127.0.0.1:22222);

FTFastTCPGen(2000, 4000, 60, 68:5b:35:c5:e9:39, 192.168.200.18,
68:5b:35:c5:e9:55, 192.168.200.17, 100, 10)
->tr
->Print(OK)
->Discard;
