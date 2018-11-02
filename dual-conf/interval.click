// In aqua07
// aqua01 -> aqua07 -> aqua01


FromDPDKDevice(1, 0)
-> MarkIPHeader(14)
-> IPFilter(allow udp && src 1.0.0.0/16)
-> count::Counter
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.1.108, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
-> StoreEtherAddress(f4:52:14:69:a5:20, dst)
-> ToDPDKDevice(1, 0);
