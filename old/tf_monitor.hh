#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "transmitter.hh"

CLICK_DECLS

#define DEFAULT_INDEX 0
#define DEFAULT_VALUE 1

class TFMonitor : public Element {
private:
    int _index;
    int _for_count;

    Transmitter *_trans;

    inline void _init_transmitter();

public:
    TFMonitor  ();

    ~TFMonitor ();

    const char *class_name() const { return "TFMonitor "; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &, ErrorHandler *);

    Packet *simple_action(Packet *);
};

CLICK_ENDDECLS