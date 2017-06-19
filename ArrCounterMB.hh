#pragma once

#include "FTTypes.hh"
#include <click/config.h>
#include <click/element.hh>
#include <fstream>
#include <map>
#include <vector>

#define INIT_COUNTER 0
#define COUNTER_MB_CLASS_NAME "ArrCounterMB"
#define COUNTER "c"

CLICK_DECLS

class ArrCounterMB : public Element {
private:
    std::vector<unsigned int> _counters;
    FTMBId _id;
//    int _new_count;
    int _counter;

public:
    ArrCounterMB();

    ~ArrCounterMB();

    const char *class_name() const { return "ArrCounterMB"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    int configure(Vector<String> &conf, ErrorHandler *errh);
};

CLICK_ENDDECLS