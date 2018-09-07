AddressInfo(sender 192.168.1.101);
array::LockFreeArray;

elementclass CounterBlock {
$index |
    input
    -> MarkIPHeader(14)
//    -> IPFilter(allow src sender)
//    -> IPPrint($index)
    -> NFLockFreeCounter(INDEX $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress(192.168.1.107, src)
    -> StoreIPAddress(192.168.1.101, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:72, dst)
    -> output
}

fd0::FromDPDKDevice(0,0);
fd1::FromDPDKDevice(0,1);

//td::ToDPDKDevice(0);
td0::ToDPDKDevice(0,0);
td1::ToDPDKDevice(0,1);

StaticThreadSched(fd0 0,fd1 1)

fd0
-> CounterBlock(0)
//-> td;
-> td0;

fd1
-> CounterBlock(1)
//-> td;
-> td1;
