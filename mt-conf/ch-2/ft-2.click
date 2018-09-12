// In aqua08
// aqua07 -> aqua08 -> aqua01

shared_state::SharedLockFreeState(CHAIN 2, ID 1, F 1);

elementclass FTBlock {
$index,$out,$forwarder |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct
    -> buffer::Buffer;

    // To the outside world
    buffer[0]
    -> IPPrint("To outside world!")
    -> StoreIPAddress($out, src)
    -> StoreIPAddress(192.168.1.101, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:72, dst)
    -> output;

    // To the forwarder
    buffer[1]
    -> IPPrint("To forwarder!")
    -> StoreIPAddress($forwarder, src)
    -> StoreIPAddress(192.168.1.107, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, dst)
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
-> FTBlock(0,1.2.1.1,2.0.1.1)
-> td1;

// fd2
// -> FTBlock(1,1.2.2.2,2.0.2.2)
// -> td2;
//
// fd3
// -> FTBlock(2,1.2.3.3,2.0.3.3)
// -> td3;
//
// fd4
// -> FTBlock(3,1.2.4.4,2.0.4.4)
// -> td4;
//
// fd5
// -> FTBlock(4,1.2.5.5,2.0.5.5)
// -> td5;
//
// fd6
// -> FTBlock(5,1.2.6.6,2.0.6.6)
// -> td6;
//
// fd7
// -> FTBlock(6,1.2.7.7,2.0.7.7)
// -> td7;
//
// fd8
// -> FTBlock(7,1.2.8.8,2.0.8.8)
// -> td8;
