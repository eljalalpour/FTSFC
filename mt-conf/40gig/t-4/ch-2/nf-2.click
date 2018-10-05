// In aqua08
// aqua07 -> aqua08 -> aqua01

array::AtomicArray;

elementclass NFBlock {
$index,$src_ip |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
//    -> IPPrint($index)
    -> NFAtomicCounter(INDEX $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(10.70.0.1, dst)
    -> StoreEtherAddress(f4:52:14:5a:90:70, src)
    -> StoreEtherAddress(f4:52:14:69:a5:20, dst)
    -> output
}

fd1::FromDPDKDevice(0,0);
fd2::FromDPDKDevice(0,1);
fd3::FromDPDKDevice(0,2);
fd4::FromDPDKDevice(0,3);

td1::ToDPDKDevice(0,0);
td2::ToDPDKDevice(0,1);
td3::ToDPDKDevice(0,2);
td4::ToDPDKDevice(0,3);

StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3)

fd1
-> NFBlock(0,1.2.1.1)
-> td1;

fd2
-> NFBlock(0,1.2.2.2)
-> td2;

fd3
-> NFBlock(0,1.2.3.3)
-> td3;

fd4
-> NFBlock(3,1.2.4.4)
-> td4;
