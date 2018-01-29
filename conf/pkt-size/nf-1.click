AddressInfo(sender 192.168.233.6);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
-> Counter
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.7, src)
-> StoreIPAddress(192.168.233.8, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst)
-> ToDPDKDevice(0);