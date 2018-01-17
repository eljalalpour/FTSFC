AddressInfo(sender 192.168.233.8);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
-> se::FTStateElement(ID 2, F 1)
-> MarkIPHeader(14)
-> CounterMB(ID 2)
-> MarkIPHeader(14)
-> [1]se;

se[1]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.9, src)
-> StoreIPAddress(192.168.233.10, dst)
-> StoreEtherAddress(0c:c4:7a:73:f9:ec, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:18, dst)
-> ToDPDKDevice(0);
