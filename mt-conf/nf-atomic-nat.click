array::AtomicArray;

elementclass CounterBlock {
$index |
    input
    -> MarkIPHeader(14)
//    -> IPPrint($index)
    -> NFAtomicNAT()
    -> MarkIPHeader(14)
    -> StoreIPAddress(192.168.1.107, src)
    -> StoreIPAddress(192.168.1.101, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:72, dst)
    -> output
}

fd0::FromDPDKDevice(0,0);
fd1::FromDPDKDevice(0,1);
fd2::FromDPDKDevice(0,2);
fd3::FromDPDKDevice(0,3);
fd4::FromDPDKDevice(0,4);
fd5::FromDPDKDevice(0,5);
fd6::FromDPDKDevice(0,6);
fd7::FromDPDKDevice(0,7);

td0::ToDPDKDevice(0,0);
td1::ToDPDKDevice(0,1);
td2::ToDPDKDevice(0,2);
td3::ToDPDKDevice(0,3);
td4::ToDPDKDevice(0,4);
td5::ToDPDKDevice(0,5);
td6::ToDPDKDevice(0,6);
td7::ToDPDKDevice(0,7);

//td::ToDPDKDevice(0);

StaticThreadSched(
    fd0 0,
    fd1 1,
    fd2 3,
    fd3 3,
    fd4 4,
    fd5 5,
    fd6 6,
    fd7 7)

fd0
-> CounterBlock(0)
-> td0;
//-> td;


fd1
-> CounterBlock(1)
-> td1;
//-> td;

fd2
-> CounterBlock(2)
-> td2;
//-> td;

fd3
-> CounterBlock(3)
-> td3;
//-> td;

fd4
-> CounterBlock(4)
-> td4;
//-> td;

fd5
-> CounterBlock(5)
-> td5;
//-> td;

fd6
-> CounterBlock(6)
-> td6;
//-> td;

fd7
-> CounterBlock(7)
-> td7;
//-> td;
