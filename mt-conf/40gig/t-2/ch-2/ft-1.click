// In aqua07
// aqua01 -> aqua07 -> aqua08

shared_state::SharedState(CHAIN 2, ID 0, F 1);

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
    -> [0]forwarder::Forwarder(CHAIN 2);

    filter[1]
//    -> Print("buffer")
    -> [1]forwarder;

    forwarder[0]
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct(ID $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(10.70.0.8, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
    -> StoreEtherAddress(f4:52:14:5a:90:70, dst)
    -> output
}

fd1::FromDPDKDevice(0,0);
fd2::FromDPDKDevice(0,1);

td1::ToDPDKDevice(0,0);
td2::ToDPDKDevice(0,1);

StaticThreadSched(fd1 0, fd2 1);

fd1
-> FTBlock(0,1.1.1.1)
-> td1;

fd2
-> FTBlock(1,1.1.2.2)
-> td2;
