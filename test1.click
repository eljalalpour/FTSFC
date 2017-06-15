require(package "FTSFC");

AddressInfo(sender 10.70.0.9);

FromDevice(p1p1)
->CheckIPHeader(14)
->IPFilter(allow src sender)
->ctr::Counter()
->Print(ok)
->Discard;
