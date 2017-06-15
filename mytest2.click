require(package "FTSFC");

FTFastTCPGen(20, 200, 1000, e4:1d:2d:13:9e:c0, 10.70.0.6, e4:1d:2d:13:9e:d0, 10.70.0.7, 100, 10)
-> CheckIPHeader(14)
-> Print(ok)
//-> FTLoggerElement(experiments/nf/src.20.1000.txt)
-> ToDevice(p1p1);
