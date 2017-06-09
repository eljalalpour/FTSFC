#pragma once

#include "FTTypes.hh"
#include <click/config.h>
#include <click/element.hh>
#include <fstream>
#include <map>

#define INIT_COUNTER 0
#define COUNTER_MB_CLASS_NAME "NFCounterMB"
#define COUNTER "c"

CLICK_DECLS

class NFCounterMB : public Element {
private:
    unsigned int _counter;
    FTMBId _id;

public:
    NFCounterMB();

    ~NFCounterMB();

    const char *class_name() const { return "NFCounterMB"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    int configure(Vector<String> &conf, ErrorHandler *errh);

    unsigned int counter();

    void write_to_file();
};

CLICK_ENDDECLS