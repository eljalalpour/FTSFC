// In aqua08
// aqua07 -> aqua08 -> aqua01

shared_state::SharedLockFreeState(ID 1, F 1);

elementclass FTBlock {
$index,$src_ip |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
    -> PMProcess
    -> FTLockFreeCounter($index)
    -> PMConstruct
    -> MarkIPHeader(14)
    -> buffer::Buffer;

    // To the outside world
    buffer[0]
        -> MarkIPHeader(14)
        -> StoreIPAddress($src_ip, src)
        -> StoreIPAddress(192.168.1.101, dst)
        -> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
        -> StoreEtherAddress(0c:c4:7a:73:fa:72, dst)
        -> output;

    // To forwarder
    buffer[1]
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(192.168.1.107, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, dst)
    -> output;
}

FromDPDKDevice(0)
//-> Print(2)
-> MarkIPHeader(14)
-> IPFilter(allow src sender)
//-> MarkIPHeader(14)
-> se::FTStateElement(ID 1, F 1);

se[0]
//-> MarkIPHeader(14)
-> Monitor(ID 1)
//-> MarkIPHeader(14)
-> [1]se;

se[1]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.8, src)
-> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
//-> MarkIPHeader(14)
-> be::FTBufferElement;

be[0]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.6, dst)// set dst ip to receiver
-> StoreEtherAddress(0c:c4:7a:73:fa:46, dst)
//-> Print(ToDest)
-> ToDPDKDevice(0);

be[1]
-> MarkIPHeader(14)
-> StoreIPAddress(192.168.233.7, dst) // set dst ip to M1
-> StoreEtherAddress(0c:c4:7a:73:fa:54, dst)
//-> Print(ToFirst)
-> ToDPDKDevice(0);


fd1::FromDPDKDevice(0,0);
fd2::FromDPDKDevice(0,1);
fd3::FromDPDKDevice(0,2);
fd4::FromDPDKDevice(0,3);
fd5::FromDPDKDevice(0,4);
fd6::FromDPDKDevice(0,5);
fd7::FromDPDKDevice(0,6);
fd8::FromDPDKDevice(0,7);

td1::ToDPDKDevice(0,0);
td2::ToDPDKDevice(0,1);
td3::ToDPDKDevice(0,2);
td4::ToDPDKDevice(0,3);
td5::ToDPDKDevice(0,4);
td6::ToDPDKDevice(0,5);
td7::ToDPDKDevice(0,6);
td8::ToDPDKDevice(0,7);

fd1
-> FTBlock(0,1.2.1.1)
-> td1;

fd2
-> FTBlock(1,1.2.2.2)
-> td2;

fd3
-> FTBlock(2,1.2.3.3)
-> td3;

fd4
-> FTBlock(3,1.2.4.4)
-> td4;

fd5
-> FTBlock(4,1.2.5.5)
-> td5;

fd6
-> FTBlock(5,1.2.6.6)
-> td6;

fd7
-> FTBlock(6,1.2.7.7)
-> td7;

fd8
-> FTBlock(7,1.2.8.8)
-> td8;
