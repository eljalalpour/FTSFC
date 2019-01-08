// In aqua07
// aqua01 -> aqua07 -> aqua08
// aqua09 -> aqua07;

shared_state::SharedState(CHAIN 3, ID 0, F 1);

elementclass FTBlock {
$queue, $index, $src_ip |
    ld1::FromDPDKDevice(0, $queue);
    fd1::FromDPDKDevice(1, $queue);
    td1::ToDPDKDevice(1, $queue);
    forwarder::Forwarder(CHAIN 3);

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

    ld1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 2.0.0.0/16)
//    -> IPPrint("From the buffer")
    -> [1]forwarder;

    fd1
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.0.0.0/16)
//    -> IPPrint("From the source")
    -> forwarder
    -> PMProcess
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
    -> PMConstruct(ID $index)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src)
    -> StoreIPAddress(10.70.0.8, dst)
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, src)
    -> StoreEtherAddress(f4:52:14:5a:90:70, dst)
//    -> IPPrint("To second")
    -> td1;
}

FTBlock(0, 0, 1.1.1.1);




