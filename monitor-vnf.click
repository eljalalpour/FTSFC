require(package "FTSFC");

AddressInfo(sender 10.70.0.7);

FromDevice(p1p1)
->VLANDecap()
->CheckIPHeader(14)
->IPFilter(allow src sender)
->Strip(14)
->Queue
->ctr::Counter()
->StoreIPAddress(10.70.0.8, src)
//->StoreIPAddress(10.70.0.9, dst)
->StoreIPAddress(10.70.0.10, dst)
//->EtherEncap(0x0800, f4:52:14:5a:90:70, e4:1d:2d:13:9c:60)
//->VLANEncap(VLAN_ID 2)
//->EtherVLANEncap(0x0800, f4:52:14:5a:90:70, e4:1d:2d:13:9c:60, 2)
->EtherEncap(0x0800, f4:52:14:5a:90:70, e4:1d:2d:0c:89:e0)
->VLANEncap(VLAN_ID 0xf)
->ToDevice(p1p1);
