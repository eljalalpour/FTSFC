// In aqua07
// aqua01 -> aqua07 -> aqua08

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice($dev, 0);
    td1::ToDPDKDevice($dev, 0);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.0.0.0/16)
    -> FTMBInputLogger()
    -> FTMBMaster(PERIOD 200, DELAY 6, PER_PACKET 4)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)   // from aqua 7
    -> StoreIPAddress(10.70.0.8, dst) // to aqua 8
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, src) // from aqua 7
    -> StoreEtherAddress(f4:52:14:5a:90:70, dst) // to aqua 8
    -> td1;
}

MBBlock(1, 1.1.1.1);
