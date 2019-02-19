#pragma once

#include "defs.hh"
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

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    void push(int, Packet*);

    int configure(Vector<String> &, ErrorHandler *);

    void cleanup(CleanupStage) CLICK_COLD;

    void write_to_file();

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
};
