FromDPDKDevice(0)
-> MarkIPHeader(14)
-> fil::IPClassifier(src host 192.168.233.6,
                   -);
fil[1]
-> Discard;

fil[0]
-> CheckIPHeader(14)
-> StoreIPAddress(192.168.233.7, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
-> StoreIPAddress(192.168.233.6, dst)// set dst ip to receiver
-> StoreEtherAddress(0c:c4:7a:73:fa:46, dst)
-> ToDPDKDevice(0);
