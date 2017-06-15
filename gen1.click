require(package "FTSFC");

//FTFastTCPGen(60010, 600100, 1000, 0c:c4:7a:73:f8:ec, 172.19.18.110, 0c:c4:7a:73:f9:1a, 172.19.18.106, 100, 10)
FTFastTCPGen(60010, 600100, 1000, e4:1d:2d:0c:89:e0, 10.70.0.10, e4:1d:2d:13:9e:c0, 10.70.0.6, 100, 10)
//-> FTLoggerElement(experiments/nf/src.60010.1000.txt)
-> ctr::Counter()
-> ToDevice(p1p1);
