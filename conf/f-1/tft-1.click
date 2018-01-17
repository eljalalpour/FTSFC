AddressInfo(sender 192.168.233.6);

trans::Transmitter(172.19.18.112:10000); // set replica's ip and port

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
-> Monitorp(ID 0)
-> StoreIPAddress(192.168.233.7, src)
-> StoreIPAddress(192.168.233.8, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst)
-> ToDPDKDevice(0);