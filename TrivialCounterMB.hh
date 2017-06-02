#pragma once

#include "FTTypes.hh"
#include "FTClient.hh"
#include <click/config.h>
#include <click/element.hh>
#include <fstream>
#include <map>

#define INIT_COUNTER 0
#define COUNTER_MB_CLASS_NAME "TrivialCounterMB"
#define COUNTER "c"

CLICK_DECLS

class TrivialCounterMB : public Element {
private:
    unsigned int _counter;
    FTMBId _id;
    FTClient _client;
    vector<string> _ips;
    vector<uint16_t> _ports;

    void _print_ip_port_list();

public:
    TrivialCounterMB();

    ~TrivialCounterMB();

    const char *class_name() const { return "TrivialCounterMB"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    int configure(Vector<String> &conf, ErrorHandler *errh);
};

CLICK_ENDDECLS