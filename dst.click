require(package "FTSFC");

-> FromDevice(p0)
-> VLANDecap()
-> CheckIPHeader(14)
-> FTLoggerElement(dst.txt)
-> ToDump(dst.pcap);