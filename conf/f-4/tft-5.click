AddressInfo(sender 192.168.233.10);

trans::Transmitter(172.19.18.112:50000, 172.19.18.107:50000, 172.19.18.108:50000, 172.19.18.109:50000); // set replica's ip and port

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
-> Monitorp(ID 4)
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.11, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:e6, src)
-> StoreIPAddress(192.168.233.6, dst)// set dst ip to receiver
-> StoreEtherAddress(0c:c4:7a:73:fa:46, dst)
-> ToDPDKDevice(0);
