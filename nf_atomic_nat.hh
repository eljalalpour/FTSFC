#pragma once

#include "FTTypes.hh"
#include <click/config.h>
#include <click/element.hh>
#include <atomic>
#include <vector>

CLICK_DECLS

#define DEFAULT_INDEX   0

class NFAtomicNAT : public Element {
private:
    std::map<string, string> _table;

public:
    NFAtomicNAT ();

    ~NFAtomicNAT();

    const char *class_name() const { return "NFAtomicNAT "; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS