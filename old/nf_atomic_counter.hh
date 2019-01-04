#pragma once

#include "../defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <atomic>
#include <vector>

CLICK_DECLS

#define DEFAULT_INDEX   0

class NFAtomicCounter : public Element {
private:
    int _index;

public:
    NFAtomicCounter ();

    ~NFAtomicCounter();

    const char *class_name() const { return "NFAtomicCounter "; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS