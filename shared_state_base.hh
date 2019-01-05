#pragma once

#include <click/config.h>
#include <click/element.hh>
#include "shared_locks.hh"

CLICK_DECLS

class SharedStateBase : public Element {
public:
    SharedStateBase ();

    ~SharedStateBase();

    const char *class_name() const { return "SharedStateBase"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *);

    int configure(Vector<String>&, ErrorHandler*) CLICK_COLD;

    void add_handlers();

    virtual inline void increment(size_t);

    virtual inline int read(size_t);

protected:
    State _state;
    String _shared_locks_name;
    SharedLocks *_shared_locks;

    void _init_shared_locks();

    static String read_handler(Element*, void*);
};

void SharedStateBase::increment(size_t index) {
    ++_state[index];
}

int SharedStateBase::read(size_t index) {
    return _state[index];
}

CLICK_ENDDECLS
