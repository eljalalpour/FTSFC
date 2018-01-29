AddressInfo(sender 192.168.233.9);

trans::Transmitter(172.19.18.112:40000, 172.19.18.111:40000, 172.19.18.107:40000, 172.19.18.108:40000); // set replica's ip and port

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
-> Monitorp(ID 3)
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.10, src)
-> StoreIPAddress(192.168.233.11, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:18, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:e6, dst)
-> ToDPDKDevice(0);
