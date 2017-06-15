require(package "FTSFC");

AddressInfo(sender 10.70.0.8);

FromDevice(p1p1)
->CheckIPHeader(14)
->IPFilter(allow src sender)
->Strip(14)
->nat1::natnf()
->Queue
->ctr::Counter()
->StoreIPAddress(10.70.0.9, src)
->StoreIPAddress(10.70.0.6, dst)
->EtherEncap(0x0800, e4:1d:2d:13:9c:60, e4:1d:2d:13:9e:c0)
//->Print(out)
->ToDevice(p1p1);
