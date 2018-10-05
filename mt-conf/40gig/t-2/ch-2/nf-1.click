// In aqua07
// aqua01 -> aqua07 -> aqua08

array::AtomicArray;

elementclass NFBlock {
$index,$src_ip |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.0.0.0/16)
//    -> IPPrint($index)
    -> NFAtomicCounter(INDEX $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(10.70.0.8, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
    -> StoreEtherAddress(f4:52:14:5a:90:70, dst)
    -> output
}

fd1::FromDPDKDevice(0,0);
fd2::FromDPDKDevice(0,1);

td1::ToDPDKDevice(0,0);
td2::ToDPDKDevice(0,1);

StaticThreadSched(fd1 0, fd2 1);


fd1
-> NFBlock(0,1.1.1.1)
-> c1::Counter
-> td1;

fd2
-> NFBlock(0,1.1.2.2)
-> c2::Counter
-> td2;
