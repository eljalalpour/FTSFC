// In aqua08
// aqua07 -> aqua08 -> aqua01

trans::Transmitter(10.70.0.3:10000); // set replica's ip and port

elementclass TFBlock {
$index,$src_ip |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
    -> TFLockFreeCounter(INDEX $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(10.70.0.1, dst)
    -> StoreEtherAddress(f4:52:14:5a:90:70, src)
    -> StoreEtherAddress(f4:52:14:69:a5:20, dst)
//    -> IPPrint("To 0")
    -> output;
}

fd1::FromDPDKDevice(0,0);
fd2::FromDPDKDevice(0,1);

td1::ToDPDKDevice(0,0);
td2::ToDPDKDevice(0,1);

StaticThreadSched(fd1 0, fd2 1);

fd1
-> b1::TFBlock(0,1.2.1.1)
-> td1;

fd2
-> b2::TFBlock(1,1.2.2.2)
-> td2;
