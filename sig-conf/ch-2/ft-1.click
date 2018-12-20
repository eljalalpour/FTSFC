// In aqua07
// aqua01 -> aqua07 -> aqua08

shared_state::SharedLockFreeState(CHAIN 2, ID 0, F 1);

elementclass FTBlock {
$index, $src_ip |
    forwarder::Forwarder(CHAIN 2);

    input[0]
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 2.0.0.0/16)
//    -> IPPrint("From the buffer")
    -> [1]forwarder;

    input[1]
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.0.0.0/16)
//    -> IPPrint("From the source")
    -> forwarder
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct(ID $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(10.70.0.8, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
    -> StoreEtherAddress(f4:52:14:5a:90:70, dst)
//    -> IPPrint("To second")
    -> output;
}

ld1::FromDPDKDevice(0, 0);
ld2::FromDPDKDevice(0, 1);

fd1::FromDPDKDevice(1, 0);
fd2::FromDPDKDevice(1, 1);

td1::ToDPDKDevice(1, 0);
td2::ToDPDKDevice(1, 1);

b1::FTBlock(0, 1.1.1.1);
b2::FTBlock(1, 1.1.2.2);

StaticThreadSched(ld1 0, fd1 1, ld2 2, fd2 3);

ld1 -> b1;
fd1 -> [1]b1;
b1 -> td1;

ld2 -> b2;
fd2 -> [1]b2;
b2 -> td2;
