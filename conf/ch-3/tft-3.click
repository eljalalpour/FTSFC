AddressInfo(sender 192.168.233.8);

trans::Transmitter(192.168.233.12:30000); // set replica's ip and port

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.9, src)
-> StoreEtherAddress(0c:c4:7a:73:f9:ec, src)
-> StoreIPAddress(192.168.233.6, dst)// set dst ip to receiver
-> StoreEtherAddress(0c:c4:7a:73:fa:46, dst)
-> ToDPDKDevice(0);
