// In aqua08
// aqua07 -> aqua08 -> aqua01

array::LockFreeArray;

elementclass NFBlock {
$index,$src_ip |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
//    -> IPPrint($index)
    -> FTMBSim(PERIOD 20, DELAY 6, PER_PACKET 20)
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
// fd4::FromDPDKDevice(0,3);
// fd5::FromDPDKDevice(0,4);
// fd6::FromDPDKDevice(0,5);
// fd7::FromDPDKDevice(0,6);
// fd8::FromDPDKDevice(0,7);

td1::ToDPDKDevice(0,0);
td2::ToDPDKDevice(0,1);
td3::ToDPDKDevice(0,2);
// td4::ToDPDKDevice(0,3);
// td5::ToDPDKDevice(0,4);
// td6::ToDPDKDevice(0,5);
// td7::ToDPDKDevice(0,6);
// td8::ToDPDKDevice(0,7);

// StaticThreadSched(fd1 0, fd2 1);
StaticThreadSched(fd1 0, fd2 1, fd3 2)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4, fd6 5)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4, fd6 5, fd7 6)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4, fd6 5, fd7 6, fd8 7)

fd1
-> NFBlock(0,1.2.1.1)
-> td1;

fd2
-> NFBlock(0,1.2.2.2)
-> td2;

fd3
-> NFBlock(0,1.2.3.3)
-> td3;

// fd4
// -> NFBlock(3,1.2.4.4)
// -> td4;
//
// fd5
// -> NFBlock(4,1.2.5.5)
// -> td5;
//
// fd6
// -> NFBlock(5,1.2.6.6)
// -> td6;
//
// fd7
// -> NFBlock(6,1.2.7.7)
// -> td7;
//
// fd8
// -> NFBlock(7,1.2.8.8)
// -> td8;
