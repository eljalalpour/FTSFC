#pragma once

#include <click/config.h>
#include <click/element.hh>
#include "shared_locks.hh"

CLICK_DECLS

class SharedStateBase : public Element {
public:
    enum SharingLevel {
        ThreadSharing1 = 1, // no sharing
        ThreadSharing2 = 2, // 2 threads share the same counter
        ThreadSharing4 = 4, // 4 threads share the same counter
        ThreadSharing8 = 8, // all threads share the same counter
    };

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
    int _sharing_level;
    String _shared_element_name;
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
