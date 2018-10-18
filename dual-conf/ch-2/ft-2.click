// In aqua08
// aqua07 -> aqua08 -> aqua01
//                  -> aqua07

shared_state::SharedLockFreeState(CHAIN 2, ID 1, F 1);

elementclass FTBlock {
$queue, $index, $out, $forwarder |
    fd1::FromDPDKDevice(1,$queue);
    td1::ToDPDKDevice(1,$queue);
    bd1::ToDPDKDevice(0,$queue);
    buffer::Buffer(CHAIN 2);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
    -> IPPrint("From the first")
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
    -> Print("To outside world")
    -> td1;

    // To the forwarder
    buffer[1]
    -> MarkIPHeader(14)
    -> StoreIPAddress($forwarder, src)
    -> StoreIPAddress(192.168.1.107, dst)
    -> StoreEtherAddress(0c:c4:7a:73:fa:6a, src)
    -> StoreEtherAddress(0c:c4:7a:73:fa:54, dst)
    -> IPPrint("To the first")
    -> bd1;
}


FTBlock(0, 0, 1.2.1.1, 2.0.1.1);