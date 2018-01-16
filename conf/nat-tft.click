trans::Transmitter(10.0.7.12:22222); // set replica's ip
AddressInfo(sender 192.168.233.8);

FromDPDKDevice(0)
-> SetTimestamp
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
-> Strip(14)

-> natft(ID 2)

-> StoreIPAddress(192.168.233.9, src)
-> StoreIPAddress(192.168.233.6, dst)
-> EtherEncap(0x0800, 0c:c4:7a:73:f9:ec, 0c:c4:7a:73:fa:46)

-> ToDPDKDevice(0);
