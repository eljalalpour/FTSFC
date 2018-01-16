AddressInfo(sender 192.168.233.7);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src 192.168.233.7)

->ip_from_extern :: IPClassifier(
                     tcp or udp,
                     -);

ip_from_extern[1] -> Discard;

ip_from_extern[0]
-> Strip(14)
-> StoreIPAddress(192.168.233.8, src)
-> StoreIPAddress(192.168.233.9, dst)
-> EtherEncap(0x0800, 0c:c4:7a:73:fa:6a, 0c:c4:7a:73:f9:ec)
-> ToDPDKDevice(0);