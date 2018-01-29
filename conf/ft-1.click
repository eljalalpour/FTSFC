//require(package "FTSFC");

AddressInfo(source 10.70.0.6, last 10.70.0.10);
ap::FTAppenderElement();

FromDPDKDevice(0)
-> CheckIPHeader(14)
-> fil::IPClassifier(src host 10.70.0.6,
                    src host 10.70.0.9,
                    -);

fil[0] -> CheckIPHeader(14) -> [0]ap;
fil[1] -> CheckIPHeader(14) -> [1]ap;
fil[2] -> Discard;

ap
-> MarkIPHeader(14)
-> se::FTStateElement(ID 0, F 1)
-> CounterMB(ID 0)
-> CheckIPHeader(14)
-> [1]se;

se[1]
-> CheckIPHeader(14)
-> StoreIPAddress(10.70.0.7, src)
-> StoreIPAddress(10.70.0.8, dst)
-> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
-> StoreEtherAddress(f4:52:14:5a:90:70, dst)
-> ToDPDKDevice(0);
//-> Queue
//-> ToDevice(p1p1)
