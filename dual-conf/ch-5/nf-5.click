// In aqua02
// aqua10 -> aqua11 -> aqua01

_state::SharedState;

elementclass NFBlock {
$index,$src_ip |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.4.0.0/16)
//    -> IPPrint($index)
    -> NFCounter(INDEX $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(10.70.0.1, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9e:e0, src)
    -> StoreEtherAddress(f4:52:14:69:a5:20, dst)
    -> output
}

fd1::FromDPDKDevice(1,0);
// fd2::FromDPDKDevice(1,1);
// fd3::FromDPDKDevice(1,2);
// fd4::FromDPDKDevice(1,3);
// fd5::FromDPDKDevice(1,4);
// fd6::FromDPDKDevice(1,5);
// fd7::FromDPDKDevice(1,6);
// fd8::FromDPDKDevice(1,7);

td1::ToDPDKDevice(1,0);
// td2::ToDPDKDevice(1,1);
// td3::ToDPDKDevice(1,2);
// td4::ToDPDKDevice(1,3);
// td5::ToDPDKDevice(1,4);
// td6::ToDPDKDevice(1,5);
// td7::ToDPDKDevice(1,6);
// td8::ToDPDKDevice(1,7);

// StaticThreadSched(fd1 0, fd2 1);
// StaticThreadSched(fd1 0, fd2 1, fd3 2)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4, fd6 5)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4, fd6 5, fd7 6)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4, fd6 5, fd7 6, fd8 7)

fd1
-> NFBlock(0,1.5.1.1)
-> td1;

// fd2
// -> NFBlock(1,1.5.2.2)
// -> td2;
//
// fd3
// -> NFBlock(2,1.5.3.3)
// -> td3;
//
// fd4
// -> NFBlock(3,1.5.4.4)
// -> td4;
//
// fd5
// -> NFBlock(4,1.5.5.5)
// -> td5;
//
// fd6
// -> NFBlock(5,1.5.6.6)
// -> td6;
//
// fd7
// -> NFBlock(6,1.5.7.7)
// -> td7;
//
// fd8
// -> NFBlock(7,1.5.8.8)
// -> td8;
