#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "ft_shared_state.hh"
#include "shared_locks.hh"

CLICK_DECLS

#define TOTAL_TIME 200 // micro-second


class FTTran : public Element {
private:
    FTSharedState* _shared_state;
    SharedLocks *_shared_locks;

    int _index;
    inline void _init_shared_state();
    inline void _init_times(int conflict_prob);

    long long _aborts;
    long long _runs;
    long long _packets;

    int _lock_time;
    int _processing_time;

public:
    FTTran ();

    ~FTTran();

    const char *class_name() const { return "FTTran"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);

    void add_handlers() CLICK_COLD;
    static String read_param(Element *e, void *thunk) CLICK_COLD;
};

CLICK_ENDDECLS