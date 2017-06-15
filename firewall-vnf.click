require(package "FTSFC");

AddressInfo(sender 10.70.0.6);

firewall :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);
FromDevice(p1p1)
->CheckIPHeader(14)
->IPFilter(allow src sender)
-> firewall;

firewall[0] -> Discard;	// ARP queries
firewall[1] -> Discard;	// ARP responses
firewall[3] -> Discard;	// other traffic

ip_from_extern :: IPClassifier(dst tcp ssh,
                        dst tcp www or https,
                        src tcp port ftp,
                        tcp or udp,
                        -);


firewall[2] -> ip_from_extern;

ip_from_extern[0] -> Print(ssh) ->Discard; // SSH traffic (rewrite to server)
ip_from_extern[1] -> Print(https) ->Discard; // HTTP(S) traffic (rewrite to server)
ip_from_extern[2] -> Print(ftp) -> Discard; // FTP control traffic, rewrite w/tcp_rw
ip_from_extern[3] -> mo::Counter();  // other TCP or UDP traffic, rewrite to monitor
ip_from_extern[4] -> Print(discard)->Discard; // non TCP or UDP traffic is dropped


mo
//-> Print(out)
-> Queue
//->IPEncap(4, 10.70.0.7, 10.70.0.8)
//->SetIPAddress(10.70.0.8)
->Strip(14)
->StoreIPAddress(10.70.0.7, src)
->StoreIPAddress(10.70.0.8, dst)
//->EtherEncap(0x0800, e4:1d:2d:13:9e:d0, f4:52:14:5a:90:70)
//->VLANEncap(VLAN_ID 1)
->EtherVLANEncap(0x0800, e4:1d:2d:13:9e:d0, f4:52:14:5a:90:70, 1)
->ToDevice(p1p1);
