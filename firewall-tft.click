require(package "FTSFC");

trans::Transmitter(ID 0, 10.10.1.1:12, 10.1.1.1:121);
firewall :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);
FromDevice(eth6)
->VLANEncap(VLAN_ID 4)
->CheckIPHeader(18)
->ap::FTAppenderElement(4)
->VLANDecap
->CheckIPHeader(14)
-> se::FTStateElement(ID 1, VLAN_ID 4, F 1)
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

//se[0] -> CheckIPHeader(14) -> Print(tcp_or_udp) -> mo::Monitor(ID 1);

mo
-> Print(out)
-> [1]se;
//-> Discard;

se[1]
-> Discard;
