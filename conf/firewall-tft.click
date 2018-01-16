AddressInfo(sender 192.168.233.6);

trans::Transmitter(172.19.18.112:10000); // set replica's ip and port

firewall :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> SetTimestamp
-> IPFilter(allow src sender)
-> firewall;

firewall[0] -> Discard;	// ARP queries
firewall[1] -> Discard;	// ARP responses
firewall[3] -> Discard;	// other traffic

ip_from_extern :: IPClassifier(dst tcp ssh,
                        dst tcp www or https,
                        src tcp port ftp,
                        tcp or udp,
                        -);

firewall[2]
-> MarkIPHeader(14)
-> ip_from_extern;

ip_from_extern[0] -> Discard; // SSH traffic (rewrite to server)
ip_from_extern[1] -> Discard; // HTTP(S) traffic (rewrite to server)
ip_from_extern[2] -> Discard; // FTP control traffic, rewrite w/tcp_rw
ip_from_extern[4] -> Discard; // non TCP or UDP traffic is dropped

ip_from_extern[3]
-> Monitorp(ID 0)  // other TCP or UDP traffic, rewrite to monitor
-> StoreIPAddress(192.168.233.7, src)// to the next mdlbox
-> StoreIPAddress(192.168.233.8, dst)
-> EtherEncap(0x0800, 0c:c4:7a:73:fa:54, 0c:c4:7a:73:fa:6a)
-> ToDPDKDevice(0);
