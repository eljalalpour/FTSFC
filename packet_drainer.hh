#pragma once

#include "defs.hh"
#include <deque>
#include <cstring>
#include <csignal>
#include <click/config.h>
#include <click/element.hh>
#include <click/packet_anno.hh>
#include <clicknet/ether.h>
#include <clicknet/ip.h>
#include <clicknet/udp.h>

class PacketDrainer: public Element {
public:
    PacketDrainer ();

    ~PacketDrainer();

    const char *class_name() const { return "PacketDrainer"; }

    const char *port_count() const { return PORTS_0_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &, ErrorHandler *);

    Packet *pull(int);

    int configure(Vector<String> &, ErrorHandler *);

    int initialize(ErrorHandler *);

    void cleanup(CleanupStage) CLICK_COLD;

private:
    int _packets;
    int _packet_size;
    Packet *_packet;
    String _path;

    click_ether _ethh;
    struct in_addr _sipaddr;
    struct in_addr _dipaddr;

    Vector<int64_t> _measured;

    void _setup_packet();

    void _write_to_file();
};
