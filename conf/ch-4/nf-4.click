AddressInfo(sender 192.168.233.9);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> Counter
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.10, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:18, src)
-> StoreIPAddress(192.168.233.6, dst)// set dst ip to receiver
-> StoreEtherAddress(0c:c4:7a:73:fa:46, dst)
-> ToDPDKDevice(0);
