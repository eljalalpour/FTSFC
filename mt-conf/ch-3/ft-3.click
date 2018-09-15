// In aqua09
// aqua08 -> aqua09 -> aqua01

shared_state::SharedLockFreeState(CHAIN 3, ID 2, F 1);

elementclass FTBlock {
$index,$out,$forwarder |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.2.0.0/16)
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct
    -> buffer::Buffer(CHAIN 2);

    // To the outside world
    buffer[0]
    -> MarkIPHeader(14)
    -> StoreIPAddress($out, src)
    -> StoreIPAddress(192.168.1.101, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:72, dst)
//    -> Print("For latency", 300)
    -> IPPrint("To aqua01")
    -> output;

    // To the forwarder
    buffer[1]
    -> MarkIPHeader(14)
    -> StoreIPAddress($forwarder, src)
    -> StoreIPAddress(192.168.1.107, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, dst)
//    -> Print("To forwarder!")
    -> IPPrint("To aqua07")
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

fd1
-> b1::FTBlock(0,1.3.1.1,2.0.1.1)
-> td1;

// fd2
// -> b2::FTBlock(1,1.3.2.2,2.0.2.2)
// -> td2;
//
// fd3
// -> b3::FTBlock(2,1.3.3.3,2.0.3.3)
// -> td3;
//
// fd4
// -> b4::FTBlock(3,1.3.4.4,2.0.4.4)
// -> td4;
//
// fd5
// -> b5::FTBlock(4,1.3.5.5,2.0.5.5)
// -> td5;
//
// fd6
// -> b6::FTBlock(5,1.3.6.6,2.0.6.6)
// -> td6;
//
// fd7
// -> b7::FTBlock(6,1.3.7.7,2.0.7.7)
// -> td7;
//
// fd8
// -> b8::FTBlock(7,1.3.8.8,2.0.8.8)
// -> td8;
