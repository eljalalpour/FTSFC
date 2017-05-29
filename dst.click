require(package "FTSFC");

FromDevice(p2)
-> FTFilterElement(7)
-> VLANDecap
-> CheckIPHeader(14)
-> FTLoggerElement(dst.50000.txt)
-> ToDump(dst.pcap);