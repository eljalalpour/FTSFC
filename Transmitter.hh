#pragma once

#include "defs.hh"
#include "client.hh"
#include <click/config.h>
#include <click/element.hh>
#include <fstream>
#include <map>

#define INIT_COUNTER 0
#define COUNTER_MB_CLASS_NAME "Transmitter"
#define COUNTER "c"

CLICK_DECLS

class Transmitter : public Element {
private:
    unsigned int _counter;
    FTClient _client;
    vector<string> _ips;
    vector<uint16_t> _ports;

    void _print_ip_port_list();
    void _split(string& str, char dlm, vector<string>& tokens);

public:
    Transmitter();

    ~Transmitter();

    const char *class_name() const { return "Transmitter"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    bool send(FTState& state);

    int configure(Vector<String> &conf, ErrorHandler *errh);
};

CLICK_ENDDECLS