// In aqua09
// aqua08 -> aqua09 -> aqua01
//                  -> aqua07

shared_state::SharedLockFreeState(CHAIN 3, ID 2, F 1);


elementclass FTBlock {
$queue, $index, $out, $forwarder |
    fd1::FromDPDKDevice(1,$queue);
    td1::ToDPDKDevice(1,$queue);
    bd1::ToDPDKDevice(0,$queue);
    buffer::Buffer(CHAIN 3);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.2.0.0/16)
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
    -> StoreEtherAddress(e4:1d:2d:13:9c:60, src)
    -> StoreEtherAddress(f4:52:14:69:a5:20, dst)
//    -> IPPrint("To outside world")
    -> td1;

    // To the forwarder
    buffer[1]
    -> MarkIPHeader(14)
    -> StoreIPAddress($forwarder, src)
    -> StoreIPAddress(192.168.1.107, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, dst)
//    -> IPPrint("To the first")
    -> bd1;
}

FTBlock(0, 0, 1.3.1.1, 2.0.1.1);
