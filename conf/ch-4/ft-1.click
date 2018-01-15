ap::FTAppenderElement();

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> fil::IPClassifier(src host 192.168.233.6,
                    src host 192.168.233.10,
                    -);

fil[0] 
-> MarkIPHeader(14) -> [0]ap;

fil[1] 
-> MarkIPHeader(14) -> [1]ap;

fil[2] -> Discard;

ap
-> MarkIPHeader(14)
-> se::FTStateElement(ID 0, F 1)
-> CounterMB(ID 0)
-> MarkIPHeader(14)
-> [1]se;

se[1]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.7, src)
-> StoreIPAddress(192.168.233.8, dst)
-> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst)
-> ToDPDKDevice(0);
