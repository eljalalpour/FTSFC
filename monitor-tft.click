require(package "FTSFC");

trans::Transmitter(10.70.0.11:22222); // set replica's ip and port
AddressInfo(sender 10.70.0.7);

FromDevice(p1p1)
->CheckIPHeader(14)
->IPFilter(allow src sender)
->Strip(14)

->cmb::Monitorp(ID 1)

->Queue
->ctr::Counter()
->StoreIPAddress(10.70.0.8, src)
->StoreIPAddress(10.70.0.9, dst)
->EtherEncap(0x0800, f4:52:14:5a:90:70, e4:1d:2d:13:9c:60)
//->Print(ok)
->ToDevice(p1p1);
