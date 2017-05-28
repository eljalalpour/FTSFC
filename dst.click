require(package "FTSFC");

-> FromDevice(p1)
-> VLANDecap()
-> CheckIPHeader(14)
-> FTLoggerElement(dst.txt)
-> ToDump(dst.pcap);