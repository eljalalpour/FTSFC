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

class Transmitter : public Element {
private:
    Client _client;
    vector<string> _ips;
    vector<uint16_t> _ports;

    void _print_ip_port_list();
    void _split(string& str, char dlm, vector<string>& tokens);

public:
    State inoperation;

    Transmitter();

    ~Transmitter();

    const char *class_name() const { return "Transmitter"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    bool send();

    bool recv();

    int configure(Vector<String> &conf, ErrorHandler *errh);
};

CLICK_ENDDECLS