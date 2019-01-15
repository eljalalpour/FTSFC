#pragma once

#include "defs.hh"
#include "client.hh"
#include <click/config.h>
#include <click/element.hh>
#include <fstream>
#include <vector>

using std::vector;
using std::string;

CLICK_DECLS

#define MIN_PORT 10000

class Transmitter : public Element {
private:
    vector<Client> _clients; /// one client per queue
    vector<string> _ips;    /// one IP per replication factor
    int16_t _queues;

    void _print_ip_port_list(vector<string>&, vector<uint16_t>&);

public:
    State inoperation;

    Transmitter();

    ~Transmitter();

    const char *class_name() const { return "Transmitter"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    void send(int16_t);

    int configure(Vector<String> &conf, ErrorHandler *errh);
};

CLICK_ENDDECLS