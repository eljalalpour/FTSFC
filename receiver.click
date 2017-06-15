//require(package "FTSFC");

AddressInfo(sender 10.70.0.9);

FromDevice(p1p1)
//-> Print(ok1)
-> CheckIPHeader(14)
-> IPFilter(allow src sender)
-> Print(ok1)
-> ctr::Counter()
//-> FTLoggerElement(experiments/nf/dst.1000.1000.txt)
-> Discard;
