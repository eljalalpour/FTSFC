#pragma once

#include "FTTypes.hh"
#include <click/config.h>
#include <click/element.hh>
#include <fstream>
#include <map>

#define INIT_COUNTER 0
#define COUNTER_MB_CLASS_NAME "CounterMB"
#define COUNTER "c"

CLICK_DECLS

class CounterMB : public Element {
private:
    unsigned int _counter;
    FTMBId _id;

public:
    CounterMB();

    ~CounterMB();

    const char *class_name() const { return "CounterMB"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    int configure(Vector<String> &conf, ErrorHandler *errh);
};

CLICK_ENDDECLS