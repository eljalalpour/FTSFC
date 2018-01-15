AddressInfo(sender 192.168.233.7);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
//-> Print(FromFirst)
-> MarkIPHeader(14)
-> se::FTStateElement(ID 1, F 1);

se[0]
-> MarkIPHeader(14)
-> cmb::Monitor(ID 1)
-> MarkIPHeader(14)
-> [1]se;

se[1]
-> MarkIPHeader(14)
-> Strip(14)
-> StoreIPAddress(192.168.233.8, src)
-> StoreIPAddress(192.168.233.9, dst)
-> EtherEncap(0x0800, 0c:c4:7a:73:fa:6a, 0c:c4:7a:73:f9:ec)
//-> StoreIPAddress(192.168.233.6, dst)
//-> EtherEncap(0x0800, 0c:c4:7a:73:fa:6a, 0c:c4:7a:73:fa:46)
//-> Print(ToThird)
-> ToDPDKDevice(0);
