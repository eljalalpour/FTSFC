#pragma once

#include "FTTypes.hh"
#include <click/config.h>
#include <click/element.hh>
#include <fstream>
#include <map>

CLICK_DECLS

class nat : public Element {
private:
    unsigned int _counter;
    FTMBId _id;

public:
    nat();

    ~nat();

    const char *class_name() const { return "nat"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    //Packet *simple_action(Packet *p);
    void push(int port, Packet *p);

    int configure(Vector<String> &conf, ErrorHandler *errh);
};

CLICK_ENDDECLS
