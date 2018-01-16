trans::Transmitter(10.70.0.11:22222); // set replica's ip and port
firewall :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> fil::IPClassifier(src host 192.168.233.6,
	   	    src host 192.168.233.9,
	   	    //src host 192.168.233.10,
		    -);

fil[0]
//-> Print("From Begin")
-> MarkIPHeader(14)
-> [0]ap::FTAppenderElement;

fil[1]
//-> Print(FromThird)
-> [1]ap;

fil[2]
-> Discard;

ap 
-> MarkIPHeader(14)
-> se::FTStateElement(ID 0, F 1)
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
-> mo::Monitor(ID 0);  // other TCP or UDP traffic, rewrite to monitor

mo
-> [1]se;

se[1]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.7, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
-> StoreIPAddress(192.168.233.8, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst)
//-> IPPrint("To Second...")
-> ToDPDKDevice(0);

//ap
//-> MarkIPHeader(14)
//-> StoreIPAddress(192.168.233.7, src)
//-> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
//-> StoreIPAddress(192.168.233.6, dst)
//-> StoreEtherAddress(0c:c4:7a:73:fa:46, dst)
//-> ToDPDKDevice(0);
