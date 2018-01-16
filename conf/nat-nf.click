AddressInfo(sender 192.168.233.8);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src 192.168.233.8)
-> Strip(14)
-> nat1::natnf()
-> StoreIPAddress(192.168.233.9, src)
-> StoreIPAddress(192.168.233.6, dst)
-> EtherEncap(0x0800, 0c:c4:7a:73:f9:ec, 0c:c4:7a:73:fa:46)
//-> Print(ok)
-> ToDPDKDevice(0);
