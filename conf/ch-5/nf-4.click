AddressInfo(sender 192.168.233.9);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> Counter
-> MarkIPHeader(14)
-> [1]se;

se[1]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.10, src)
-> StoreIPAddress(192.168.233.11, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:18, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:e6, dst)
-> ToDPDKDevice(0);
