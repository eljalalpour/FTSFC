#pragma once

#include <click/config.h>
#include <click/element.hh>
#inculde <atomic>
#inculde <vector>

CLICK_DECLS

#define DEFAULT_INDEX   0

class NFAtomicCounter : public Element {
private:
    int _index;

public:
    NFLockFreeCounter () : _index(DEFAULT_INDEX) { };

    ~CounterNF() { };

    const char *class_name() const { return "NFAtomicCounter "; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS