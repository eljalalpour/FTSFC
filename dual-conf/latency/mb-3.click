array::LockFreeArray;

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice($dev, 0);
    td1::ToDPDKDevice($dev, 0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.2.0.0/16)
    -> FTMBInputLogger()
    -> NFMonitor(INDEX 0, FOR 15)
    -> FTMBMaster(PERIOD 200, DELAY 6, PER_PACKET 3)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)    // from aqua 9
    -> StoreIPAddress(192.168.1.110, dst) // to aqua 10
    -> StoreEtherAddress(0c:c4:7a:73:f9:ec, src) // from aqua 9
    -> StoreEtherAddress(0c:c4:7a:73:fa:18, dst) // to aqua 10
    -> td1;
}

MBBlock(0, 1.3.1.1);
