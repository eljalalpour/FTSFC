#pragma once

#include "FTClient.hh"
#include "FTTypes.hh"
#include <click/config.h>
#include <click/element.hh>
#include <fstream>
#include <map>

CLICK_DECLS

class natft : public Element {
private:
    unsigned int _counter;

    FTMBId _id;

    std::map<string, string> _table;

//    FTClient _client;

    vector<string> _ips;

    vector<uint16_t> _ports;

public:
    natft();

    ~natft();

    const char *class_name() const { return "natft"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    //Packet *simple_action(Packet *p);
    void push(int port, Packet *p);

    int configure(Vector<String> &conf, ErrorHandler *errh);

    void _print_ip_port_list();

    void _split(string& str, char dlm, vector<string>& tokens);
};

CLICK_ENDDECLS
