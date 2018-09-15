// In aqua07
// aqua01 -> aqua07 -> aqua08

shared_state::SharedLockFreeState(CHAIN 2, ID 0, F 1);

elementclass FTBlock {
$index,$src_ip |
    input
    -> MarkIPHeader(14)
    -> filter::IPClassifier(src net 1.0.0.0/16,
                            src net 2.0.0.0/16,
                            -);

    filter[2]
    -> Discard;

    filter[0]
//    -> Print("latency", 300)
    -> [0]forwarder::Forwarder();

    filter[1]
//    -> Print("buffer")
    -> [1]forwarder;

    forwarder[0]
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(192.168.1.108, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst)
    -> output
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
-> FTBlock(0,1.1.1.1)
-> td1;

// fd2
// -> FTBlock(1,1.1.2.2)
// -> td2;
//
// fd3
// -> FTBlock(2,1.1.3.3)
// -> td3;
//
// fd4
// -> FTBlock(3,1.1.4.4)
// -> td4;
//
// fd5
// -> FTBlock(4,1.1.5.5)
// -> td5;
//
// fd6
// -> FTBlock(5,1.1.6.6)
// -> td6;
//
// fd7
// -> FTBlock(6,1.1.7.7)
// -> td7;
//
// fd8
// -> FTBlock(7,1.1.8.8)
// -> td8;