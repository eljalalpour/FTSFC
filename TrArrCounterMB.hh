#pragma once

#include "FTTypes.hh"
#include <click/config.h>
#include <click/element.hh>
#include <fstream>
#include <map>
#include <vector>

#define INIT_COUNTER 0
#define COUNTER_MB_CLASS_NAME "TrArrCounterMB"
#define COUNTER "c"

CLICK_DECLS

class TrArrCounterMB : public Element {
private:
    std::vector<unsigned int> _counters;
    FTMBId _id;

public:
    TrArrCounterMB();

    ~TrArrCounterMB();

    const char *class_name() const { return "TrArrCounterMB"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    int configure(Vector<String> &conf, ErrorHandler *errh);
};

CLICK_ENDDECLS