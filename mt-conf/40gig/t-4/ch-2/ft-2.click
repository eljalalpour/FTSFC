// In aqua08
// aqua07 -> aqua08 -> aqua01
//                  -> aqua07

shared_state::SharedLockFreeState(CHAIN 2, ID 1, F 1);

elementclass FTBlock {
$index,$out,$forwarder |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct(ID $index)
    -> buffer::Buffer(CHAIN 2);

    // To the outside world
    buffer[0]
    -> MarkIPHeader(14)
    -> StoreIPAddress($out, src)
    -> StoreIPAddress(10.70.0.1, dst)
    -> StoreEtherAddress(f4:52:14:5a:90:70, src)
    -> StoreEtherAddress(f4:52:14:69:a5:20, dst)
//    -> Print("latency", 300)
    -> output;

    // To the forwarder
    buffer[1]
//    -> Truncate(380)
    -> MarkIPHeader(14)
    -> StoreIPAddress($forwarder, src)
    -> StoreIPAddress(10.70.0.7, dst)
    -> StoreEtherAddress(f4:52:14:5a:90:70, src)
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, dst)
//    -> Print("to 1")
    -> output;
}

fd1::FromDPDKDevice(0,0);
fd2::FromDPDKDevice(0,1);
fd3::FromDPDKDevice(0,2);
fd4::FromDPDKDevice(0,3);

td1::ToDPDKDevice(0,0);
td2::ToDPDKDevice(0,1);
td3::ToDPDKDevice(0,2);
td4::ToDPDKDevice(0,3);

StaticThreadSched(fd1 0, fd2 1, fd3 2, fd4 3)

fd1
-> b1::FTBlock(0,1.2.1.1,2.0.1.1)
-> td1;

fd2
-> b2::FTBlock(1,1.2.2.2,2.0.2.2)
-> td2;

fd3
-> b3::FTBlock(2,1.2.3.3,2.0.3.3)
-> td3;

fd4
-> b4::FTBlock(3,1.2.4.4,2.0.4.4)
-> td4;

