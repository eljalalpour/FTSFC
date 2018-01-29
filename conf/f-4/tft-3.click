AddressInfo(sender 192.168.233.8);

trans::Transmitter(172.19.18.112:30000, 172.19.18.110:30000, 172.19.18.111:30000, 172.19.18.107:30000); // set replica's ip and port

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
-> Monitorp(ID 2)
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.9, src)
-> StoreIPAddress(192.168.233.10, dst)
-> StoreEtherAddress(0c:c4:7a:73:f9:ec, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:18, dst)
-> ToDPDKDevice(0);
