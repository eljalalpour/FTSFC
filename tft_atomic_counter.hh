#pragma once

#include "FTTypes.hh"
#include <click/config.h>
#include <click/element.hh>
#include <atomic>
#include <vector>
#include <string>

CLICK_DECLS

#define DEFAULT_INDEX 0
#define DEFAULT_VALUE 1

class TFTAtomicCounter : public Element {
private:
    int _index;
    string _key;
    string _val;

public:
    TFTAtomicCounter  ();

    ~TFTAtomicCounter ();

    const char *class_name() const { return "TFTAtomicCounter "; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS