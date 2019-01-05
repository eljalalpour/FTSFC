// In aqua08
// aqua07 -> aqua08 -> aqua01
//                  -> aqua07

shared_state::SharedState(CHAIN 2, ID 1, F 1);

elementclass FTBlock {
$index, $out, $forwarder |
    buffer::Buffer(BATCH 1, CHAIN 2);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
//    -> IPPrint("From the first")
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct(ID $index)
    -> buffer;

    // To the outside world
    buffer[0]
    -> MarkIPHeader(14)
    -> StoreIPAddress($out, src)
    -> StoreIPAddress(10.70.0.1, dst)
    -> StoreEtherAddress(f4:52:14:5a:90:70, src)
    -> StoreEtherAddress(f4:52:14:69:a5:20, dst)
//    -> IPPrint("To outside world")
    -> [0]output;

    // To the forwarder
    buffer[1]
    -> MarkIPHeader(14)
    -> StoreIPAddress($forwarder, src)
    -> StoreIPAddress(192.168.1.107, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, dst)
//    -> IPPrint("To the first")
    -> [1]output;
}


fd1::FromDPDKDevice(1, 0);
fd2::FromDPDKDevice(1, 1);

ld1::FromDPDKDevice(0, 0);
ld2::FromDPDKDevice(0, 1);

td1::ToDPDKDevice(1, 0);
td2::ToDPDKDevice(1, 1);

bd1::ToDPDKDevice(0, 0);
bd2::ToDPDKDevice(0, 1);

b1::FTBlock(0, 1.2.1.1, 2.0.1.1);
b2::FTBlock(1, 1.2.2.2, 2.0.2.2)

StaticThreadSched(fd1 0, fd2 1);

ld1 -> b1;
b1[0] -> td1;
b1[1] -> bd1;

ld2 -> b2;
b1[0] -> td2;
b1[1] -> bd2;
