require(package "FTSFC");

-> FromDevice(p2)
-> VLANDecap
-> CheckIPHeader(14)
-> FTLoggerElement(dst.2000.txt)
-> ToDump(dst.pcap);