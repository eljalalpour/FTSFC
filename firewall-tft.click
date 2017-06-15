require(package "FTSFC");

AddressInfo(sender 10.70.0.6);
trans::Transmitter(10.70.0.10:22222); // one replica's ip and port

firewall :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);

FromDevice(p1p1)
-> CheckIPHeader(14)
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


firewall[2] -> Strip(14) -> CheckIPHeader -> ip_from_extern;

ip_from_extern[0] -> Discard; // SSH traffic (rewrite to server)
ip_from_extern[1] -> Discard; // HTTP(S) traffic (rewrite to server)
ip_from_extern[2] -> Discard; // FTP control traffic, rewrite w/tcp_rw
ip_from_extern[3] -> mo::Monitorp(ID 1);  // other TCP or UDP traffic, rewrite to monitor
ip_from_extern[4] -> Discard; // non TCP or UDP traffic is dropped

mo
//-> Print(out)
-> ctr::Counter()
-> Queue
-> StoreIPAddress(10.70.0.7, src)// to the next mdlbox
-> StoreIPAddress(10.70.0.8, dst)
-> EtherEncap(0x0800, e4:1d:2d:13:9e:d0, f4:52:14:5a:90:70)
-> ToDevice(p1p1);
