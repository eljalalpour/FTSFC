// In aqua08
// aqua07 -> aqua08 -> aqua09

//
// Check-list:
// Transmitter dest ip
// IPFilter src IP
// StoreIPAddress destination IP
// StoreEtherAddress src
// StoreEtherAddress dst
// TFBlock elements second arguments
//

trans::Transmitter(10.70.0.3:10000, 10.70.0.4:20000, 10.70.0.5:30000, 10.70.0.12:40000); // set replica's ip and port

elementclass TFBlock {
$index,$src_ip |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
    -> TFLockFreeCounter(INDEX $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(192.168.1.109, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
    -> StoreEtherAddress(0c:c4:7a:73:f9:ec, dst)
//    -> IPPrint("To 3")
    -> output;
}

fd1::FromDPDKDevice(0,0);
// fd2::FromDPDKDevice(0,1);
// fd3::FromDPDKDevice(0,2);
// fd4::FromDPDKDevice(0,3);
// fd5::FromDPDKDevice(0,4);
// fd6::FromDPDKDevice(0,5);
// fd7::FromDPDKDevice(0,6);
// fd8::FromDPDKDevice(0,7);


td1::ToDPDKDevice(0,0);
// td2::ToDPDKDevice(0,1);
// td3::ToDPDKDevice(0,2);
// td4::ToDPDKDevice(0,3);
// td5::ToDPDKDevice(0,4);
// td6::ToDPDKDevice(0,5);
// td7::ToDPDKDevice(0,6);
// td8::ToDPDKDevice(0,7);

// StaticThreadSched(fd1 0, fd2 1);
// StaticThreadSched(fd1 0, fd2 1, fd3 2)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4, fd6 5)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4, fd6 5, fd7 6)
// StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3, fd5 4, fd6 5, fd7 6, fd8 7)

fd1
-> b1::TFBlock(0,1.2.1.1)
-> td1;

// fd2
// -> b2::TFBlock(1,1.2.2.2)
// -> td2;
//
// fd3
// -> b3::TFBlock(2,1.2.3.3)
// -> td3;
//
// fd4
// -> b4::TFBlock(3,1.2.4.4)
// -> td4;
//
// fd5
// -> b5::TFBlock(4,1.2.5.5)
// -> td5;
//
// fd6
// -> b6::TFBlock(5,1.2.6.6)
// -> td6;
//
// fd7
// -> b7::TFBlock(6,1.2.7.7)
// -> td7;
//
// fd8
// -> b8::TFBlock(7,1.2.8.8)
// -> td8;
