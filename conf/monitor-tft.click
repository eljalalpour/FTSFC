trans::Transmitter(10.70.0.11:22222); // set replica's ip and port
AddressInfo(sender 192.168.233.7);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
-> Strip(14)

-> cmb::Monitorp(ID 1)

-> StoreIPAddress(192.168.233.8, src)
-> StoreIPAddress(192.168.233.9, dst)
-> EtherEncap(0x0800, 0c:c4:7a:73:fa:6a, 0c:c4:7a:73:f9:ec)

-> ToDPDKDevice(0);
