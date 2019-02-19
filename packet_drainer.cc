#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "packet_drainer.hh"

CLICK_DECLS

PacketDrainer* logger;

void signal_handler(int signal) {
    LOG("Interrupt signal (%d) received. Writing packets timestamps to file!", signal);

    logger->write_to_file();
    exit(signal);
}

PacketDrainer::PacketDrainer () {
    logger = this;
};

PacketDrainer::~PacketDrainer() { };

void PacketDrainer::_setup_packet() {
    _packet = Packet::make(pkt_size);
    memcpy(_packet->data(), &_ethh, MAC_HEAD_SIZE);
    click_ip *ip = reinterpret_cast<click_ip *>(_packet->data() + MAC_HEAD_SIZE);
    click_udp *udp = reinterpret_cast<click_udp *>(ip + UDP_HEAD_OFFSET_AFTER_MAC_HEAD);

    // set up IP header
    ip->ip_v = 4;
    ip->ip_hl = sizeof(click_ip) >> 2;
    ip->ip_len = htons(pkt_size - MAC_HEAD_SIZE);
    ip->ip_id = 0;
    ip->ip_p = IP_PROTO_UDP;
    ip->ip_src = _sipaddr;
    ip->ip_dst = _dipaddr;
    ip->ip_tos = 0;
    ip->ip_off = 0;
    ip->ip_ttl = 250;
    ip->ip_sum = 0;
    ip->ip_sum = click_in_cksum((unsigned char *)ip, sizeof(click_ip));
    _packet->set_dst_ip_anno(IPAddress(_dipaddr));
    _packet->set_ip_header(ip, sizeof(click_ip));

    // set up UDP header
    udp->uh_sport = htons(10000);
    udp->uh_dport = htons(10000);
    udp->uh_sum = 0;
    unsigned short len = _packet_size - MAC_HEAD_SIZE - sizeof(click_ip);
    udp->uh_ulen = htons(len);
    udp->uh_sum = 0;

    auto payload_offset = MAC_HEAD_SIZE + UDP_HEAD_OFFSET_AFTER_MAC_HEAD + sizeof(*click_udp);

    String _data = "Random bullshit in a packet, at least 64 bytes long. Well, now it is.";
    StringAccum sa;
    while (sa.length() < _packet_size - payload_offset)
        sa << _data;

    memcpy(_packet->data() + payload_offset, sa.data(), len);
}

int PacketDrainer::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh)
                .read("PACKETS", _packets)
                .read("SIZE", _packet_size)
                .read("PATH", _path)
                .read_mp("SRCETH", EtherAddressArg(), _ethh.ether_shost)
                .read_mp("DSTETH", EtherAddressArg(), _ethh.ether_dhost)
                .read_mp("SRCIP", _sipaddr)
                .read_mp("DSTIP", _dipaddr)
                .complete() < 0)
        return -1;

    LOG("PacketDrainer: Number of packets is %d, and the packet size is $d!\n", _packets, _packet_size);

    _setup_packet();

    return 0;
}

void PacketDrainer::_write_to_file() {
    std::ofstream ofs(_path.c_str(), std::ofstream::out);
    for (int i = 0; i < _measured.size(); ++i) {
        ofs << _measured[i] << std::endl;
    }//for
    ofs.close();

    LOG("Wrote stats into '%s'!", _path.c_str());
}

void PacketDrainer::push(int, Packet*) {
    Packet** _packets = new *Packet[_packets];

    for (int i = 0; i < _packets; ++i) {
        _packets.push(_packet->clone());
    }//for

    auto before = CLOCK_NOW;
    for (int i = 0; i < _packets; ++i) {
        output(0).push(_packets[i]);
    }//for
    auto diff = CLOCK_NOW - before;

    _measured.push_back(diff);
}

void PacketDrainer::cleanup(CleanupStage) {
    if (_packet)
        _packet->kill();
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PacketDrainer)
