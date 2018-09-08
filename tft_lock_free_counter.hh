#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

#define DEFAULT_INDEX 0
#define DEFAULT_VALUE 1

class TFTLockFreeCounter : public Element {
private:
    int _index;
    string _key;
    string _val;

public:
    TFTLockFreeCounter  ();

    ~TFTLockFreeCounter ();

    const char *class_name() const { return "TFTLockFreeCounter "; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS