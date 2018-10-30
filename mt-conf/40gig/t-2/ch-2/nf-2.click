// In aqua08
// aqua07 -> aqua08 -> aqua01

array::LockFreeArray;

elementclass NFBlock {
$index,$src_ip |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
//    -> IPPrint($index)
    -> NFLockFreeCounter(INDEX $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(10.70.0.1, dst)
    -> StoreEtherAddress(f4:52:14:5a:90:70, src)
    -> StoreEtherAddress(f4:52:14:69:a5:20, dst)
    -> output
}

fd1::FromDPDKDevice(0,0);
fd2::FromDPDKDevice(0,1);

td1::ToDPDKDevice(0,0);
td2::ToDPDKDevice(0,1);

StaticThreadSched(fd1 0, fd2 1);

fd1
-> NFBlock(0,1.2.1.1)
-> td1;

fd2
-> NFBlock(0,1.2.2.2)
-> td2;
