// In aqua07
// aqua01 -> aqua07 -> aqua08

array::AtomicArray;

elementclass NFBlock {
$index,$src_ip |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 22.1.0.0/16)
    -> IPPrint($index)
    -> MarkIPHeader(14)
    -> NFAtomicCounter(INDEX $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(192.168.1.108, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst)
    -> output
}

fd1::FromDPDKDevice(0,0);
fd2::FromDPDKDevice(0,1);
fd3::FromDPDKDevice(0,2);
fd4::FromDPDKDevice(0,3);
fd5::FromDPDKDevice(0,4);
fd6::FromDPDKDevice(0,5);
fd7::FromDPDKDevice(0,6);
fd8::FromDPDKDevice(0,7);

td1::ToDPDKDevice(0,0);
td2::ToDPDKDevice(0,1);
td3::ToDPDKDevice(0,2);
td4::ToDPDKDevice(0,3);
td5::ToDPDKDevice(0,4);
td6::ToDPDKDevice(0,5);
td7::ToDPDKDevice(0,6);
td8::ToDPDKDevice(0,7);

fd1
-> NFBlock(0,22.1.2.1)
-> td1;

fd2
-> NFBlock(0,22.1.2.2)
-> td2;

fd3
-> NFBlock(0,22.1.2.3)
-> td3;

fd4
-> NFBlock(0,22.1.2.4)
-> td4;

fd5
-> NFBlock(0,22.1.2.5)
-> td5;

fd6
-> NFBlock(0,22.1.2.6)
-> td6;

fd7
-> NFBlock(0,22.1.2.7)
-> td7;

fd8
-> NFBlock(0,22.1.2.8)
-> td8;