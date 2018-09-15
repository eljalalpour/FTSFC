// In aqua08
// aqua07 -> aqua08 -> aqua01

trans::Transmitter(10.70.0.3:10000); // set replica's ip and port

elementclass TFBlock {
$index,$out,$forwarder |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
    -> TFLockFreeCounter(INDEX $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($out, src)
    -> StoreIPAddress(192.168.1.101, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:72, dst)
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
-> b1::TFBlock(0,1.2.1.1,2.0.1.1)
-> td1;

// fd2
// -> b2::TFBlock(1,1.2.2.2,2.0.2.2)
// -> td2;
//
// fd3
// -> b3::TFBlock(2,1.2.3.3,2.0.3.3)
// -> td3;
//
// fd4
// -> b4::TFBlock(3,1.2.4.4,2.0.4.4)
// -> td4;
//
// fd5
// -> b5::TFBlock(4,1.2.5.5,2.0.5.5)
// -> td5;
//
// fd6
// -> b6::TFBlock(5,1.2.6.6,2.0.6.6)
// -> td6;
//
// fd7
// -> b7::TFBlock(6,1.2.7.7,2.0.7.7)
// -> td7;
//
// fd8
// -> b8::TFBlock(7,1.2.8.8,2.0.8.8)
// -> td8;
