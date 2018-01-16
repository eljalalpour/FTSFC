AddressInfo(sender 192.168.233.8);

trans::Transmitter(192.168.233.12:30000); // set replica's ip and port

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
-> Counter
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.9, src)
-> StoreIPAddress(192.168.233.10, dst)
-> StoreEtherAddress(0c:c4:7a:73:f9:ec, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:18, dst)
-> ToDPDKDevice(0);
