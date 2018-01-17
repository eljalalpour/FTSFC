ap::FTAppenderElement();

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> fil::IPClassifier(src host 192.168.233.6,
                    src host 192.168.233.9,
                    -);

fil[0]
-> MarkIPHeader(14) -> [0]ap;

fil[1]
-> MarkIPHeader(14) -> [1]ap;

fil[2] -> Discard;

ap
-> MarkIPHeader(14)
-> st::FTStateElement(ID 1, F 1)
-> FTCounterElement(ID 1)
-> [1]st[1]
-> StoreIPAddress(192.168.233.7, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
-> StoreIPAddress(192.168.233.6, dst) // set dst ip to M1
-> StoreEtherAddress(0c:c4:7a:73:fa:46, dst)
//-> IPPrint(REC)
-> ToDPDKDevice(0);
