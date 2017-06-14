require(package "FTSFC");

firewall :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);

AddressInfo(source 10.70.0.6, last 10.70.0.9);
ap::FTAppenderElement();

FromDevice(p1p1)
->CheckIPHeader(14)
->fil::IPClassifier(src host 10.70.0.6,
	   	    src host 10.70.0.9,
		    -);

fil[0] -> [0]ap;
fil[1] -> Print(FromAss) ->[1]ap;
fil[2] -> Discard;

ap 
-> CheckIPHeader(14)
-> filt::IPClassifier(tcp or udp,
                        -);


filt[1] -> Print(Drop) -> Discard;
filt[0]
-> se::FTStateElement(ID 1, F 1)
-> firewall;

firewall[0] -> Discard;	// ARP queries
firewall[1] -> Discard;	// ARP responses
firewall[3] -> Discard;	// other traffic

ip_from_extern :: IPClassifier(dst tcp ssh,
                        dst tcp www or https,
                        src tcp port ftp,
                        tcp or udp,
                        -);


firewall[2] -> CheckIPHeader(14) -> ip_from_extern;
//firewall[2] -> Strip(14) -> Print(beforeextern) // -> ip_from_extern;

ip_from_extern[0] -> Discard; // SSH traffic (rewrite to server)
ip_from_extern[1] -> Discard; // HTTP(S) traffic (rewrite to server)
ip_from_extern[2] -> Discard; // FTP control traffic, rewrite w/tcp_rw
ip_from_extern[3] -> mo::Monitor(ID 1);  // other TCP or UDP traffic, rewrite to monitor
ip_from_extern[4] -> Discard; // non TCP or UDP traffic is dropped

//se[0] -> CheckIPHeader(14) -> Print(tcp_or_udp) -> mo::Monitor(ID 1);

mo
-> [1]se;

se[1]
-> Strip(14)
-> CheckIPHeader
-> Queue
//-> Print(out2)
-> ctr::Counter()
-> StoreIPAddress(10.70.0.7, src)
-> StoreIPAddress(10.70.0.8, dst)
-> EtherEncap(0x0800, e4:1d:2d:13:9e:d0, f4:52:14:5a:90:70)
-> ToDevice(p1p1);
