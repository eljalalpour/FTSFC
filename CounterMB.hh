#pragma once

#include "FTTypes.hh"
#include <click/config.h>
#include <click/element.hh>
#include <fstream>
#include <map>

#define INIT_COUNTER 0
#define COUNTER_MB_CLASS_NAME "CounterMB"
#define COUNTER "counter"

CLICK_DECLS

class CounterMB : public Element {
private:
    unsigned int _counter;

public:
    CounterMB();

    ~CounterMB();

    const char *class_name() const { return "CounterMB"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS