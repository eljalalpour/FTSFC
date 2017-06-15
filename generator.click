require(package "FTSFC");

FTFastTCPGen(60010, 600100, 1000, e4:1d:2d:13:9e:c0, 10.70.0.6, e4:1d:2d:13:9e:d0, 10.70.0.7, 100, 10)
-> ctr::Counter()
-> ToDevice(p1p1);
