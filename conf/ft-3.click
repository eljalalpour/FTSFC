AddressInfo(sender 192.168.233.8);

FromDPDKDevice(0)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
//-> IPPrint(FromSecond)
-> MarkIPHeader(14)
-> se::FTStateElement(ID 2, F 1);

se[0]
-> MarkIPHeader(14)
-> CounterMB(ID 2)
-> MarkIPHeader(14)
-> [1]se;

se[1]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.9, src)
-> StoreEtherAddress(0C:C4:7A:73:F9:EC, src)
-> MarkIPHeader(14)
-> be::FTBufferElement;

be[0]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.6, dst)// set dst ip to receiver
-> StoreEtherAddress(0C:C4:7A:73:FA:46, dst)
//-> IPPrint(ToDest)
-> ToDPDKDevice(0)

be[1]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.7, dst) // set dst ip to chain begin
-> StoreEtherAddress(0C:C4:7A:73:FA:54, dst)
//-> IPPrint(ToChainBegin)
-> ToDPDKDevice(0)
