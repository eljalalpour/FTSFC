#pragma once

#include "defs.hh"
#include "client.hh"
#include <click/config.h>
#include <click/element.hh>
#include <fstream>
#include <vector>
#include <deque>
using std::vector;
using std::string;

CLICK_DECLS

#define MIN_PORT 10000
#define MAX_CLIENTS MAX_QUEUES

class Transmitter : public Element {
private:
    Client _clients[MAX_CLIENTS]; /// one client per queue
    vector<string> _ips;    /// one IP per replication factor
    int16_t _queues;
    int _batch;

    void _print_ip_port_list(vector<string>&, vector<uint16_t>&);

public:
    TFStates inoperations;

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
