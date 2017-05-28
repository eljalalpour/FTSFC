require(package "FTSFC");

-> FromDevice(p2)
-> VLANDecap()
-> CheckIPHeader(14)
-> FTLoggerElement(dst.txt)
-> ToDump(dst.pcap);