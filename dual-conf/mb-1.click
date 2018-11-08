// In aqua07
// aqua01 -> aqua07 -> aqua08

elementclass MBBlock {
$dev,$src_ip |
    fd1::FromDPDKDevice($dev, 0);
    td1::ToDPDKDevice($dev, 0);

    firewall::Classifier(12/0806 20/0001,
                             12/0806 20/0002,
                             12/0800,
                             -);

    ip_from_extern :: IPClassifier(
                        dst tcp ssh,
                        dst tcp www or https,
                        src tcp port ftp,
                        tcp or udp,
                        -);

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.0.0.0/16)
    -> firewall;

    firewall[0] -> Discard;	// ARP queries
    firewall[1] -> Discard;	// ARP responses
    firewall[3] -> Discard;	// other traffic

    firewall[2]
    -> MarkIPHeader(14)
    -> ip_from_extern;

    ip_from_extern[0] -> Discard; // SSH traffic (rewrite to server)
    ip_from_extern[1] -> Discard; // HTTP(S) traffic (rewrite to server)
    ip_from_extern[2] -> Discard; // FTP control traffic, rewrite w/tcp_rw
    ip_from_extern[4] -> Discard; // non TCP or UDP traffic is dropped

    ip_from_extern[3]
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)   // from aqua 7
    -> StoreIPAddress(10.70.0.8, dst) // to aqua 8
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, src) // from aqua 7
    -> StoreEtherAddress(f4:52:14:5a:90:70, dst) // to aqua 8
    -> td1;
}

MBBlock(1, 1.1.1.1);
