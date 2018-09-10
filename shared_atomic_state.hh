#ifndef ATOMIC_ARRAY_HH
#define ATOMIC_ARRAY_HH

#include <atomic>
#include <click/config.h>
#include <click/element.hh>
#include "defs.hh"

CLICK_DECLS

class SharedAtomicState : public Element {
public:
    SharedAtomicState ();

    ~SharedAtomicState();

    const char *class_name() const { return "SharedAtomicState"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *);

    void add_handlers();
};

CLICK_ENDDECLS
#endif
