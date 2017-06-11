#ifndef MONITORP_HH_
#define MONITORP_HH_
#pragma once

#include "FTClient.hh"
#include "FTTypes.hh"
#include <click/config.h>
#include <click/element.hh>
#include <fstream>
#include <map>

#define INIT_COUNTER 0
//#define COUNTER_MB_CLASS_NAME "CounterMB"
#define COUNTER "c"

CLICK_DECLS

class Monitorp : public Element {
private:
    unsigned int _counter;

    FTMBId _id;

    FTClient _client;

    vector<string> _ips;

    vector<uint16_t> _ports;
public:
    Monitorp();

    ~Monitorp();

    const char *class_name() const { return "Monitorp"; }

    const char *port_count() const { return PORTS_1_1; }


   const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    int configure(Vector<String> &conf, ErrorHandler *errh);

    void _print_ip_port_list();

    void _split(string& str, char dlm, vector<string>& tokens);
};

CLICK_ENDDECLS
#endif
