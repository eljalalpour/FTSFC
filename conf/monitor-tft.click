AddressInfo(sender 192.168.233.7);

trans::Transmitter(172.19.18.112:20000); // set replica's ip and port

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
-> MarkIPHeader(14)
-> Monitorp(ID 1)
-> StoreIPAddress(192.168.233.8, src)
-> StoreIPAddress(192.168.233.9, dst)
-> EtherEncap(0x0800, 0c:c4:7a:73:fa:6a, 0c:c4:7a:73:f9:ec)
-> ToDPDKDevice(0);
