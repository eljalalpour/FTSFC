FromDPDKDevice(0)
-> MarkIPHeader(14)
-> fil::IPClassifier(src host 192.168.233.6,
                    src host 192.168.233.9,
                    -);

fil[1] -> Discard;
fil[2] -> Discard;

fil[0]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.7, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
-> StoreIPAddress(192.168.233.6, dst) // set dst ip to M1
-> StoreEtherAddress(0c:c4:7a:73:fa:46, dst)
//-> IPPrint(REC)
-> ToDPDKDevice(0);
