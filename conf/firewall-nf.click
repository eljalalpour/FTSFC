AddressInfo(sender 192.168.233.6);

firewall :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> filt::IPFilter(allow src 192.168.233.6)
-> firewall;

filt[1]
-> Discard;

firewall[0] -> Discard;	// ARP queries
firewall[1] -> Discard;	// ARP responses
firewall[3] -> Discard;	// other traffic

ip_from_extern :: IPClassifier(dst tcp ssh,
                        dst tcp www or https,
                        src tcp port ftp,
                        tcp or udp,
                        -);

firewall[2]
-> ip_from_extern;

ip_from_extern[0] -> Discard; // SSH traffic (rewrite to server)
ip_from_extern[1] -> Discard; // HTTP(S) traffic (rewrite to server)
ip_from_extern[2] -> Discard; // FTP control traffic, rewrite w/tcp_rw
ip_from_extern[4] -> Discard; // non TCP or UDP traffic is dropped

ip_from_extern[3]
-> Strip(14)
-> StoreIPAddress(192.168.233.7, src)
-> StoreIPAddress(192.168.233.8, dst)
-> EtherEncap(0x0800, 0c:c4:7a:73:fa:54, 0c:c4:7a:73:fa:6a)
//-> Print(ToSecond)
-> ToDPDKDevice(0);