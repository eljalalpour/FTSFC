AddressInfo(sender 192.168.233.7);

trans::Transmitter(192.168.233.12:20000); // set replica's ip and port

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
-> Monitorp(ID 1)
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.8, src)
-> StoreIPAddress(192.168.233.9, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
-> StoreEtherAddress(0c:c4:7a:73:f9:ec, dst)
-> ToDPDKDevice(0);
