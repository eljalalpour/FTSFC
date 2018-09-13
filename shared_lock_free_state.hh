#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
//#include <mutex>

CLICK_DECLS

///
/// ShareLockFreeState maintains the state that is shared among multiple Replica class instances,
/// and performs low-level state management operations.
///

class SharedLockFreeState : public Element {
private:
    int _id;
    int _failure_count;
    int _chain_len;
    LogTable _log_table;
    CommitMemory _commit_memory;
    Util _util;

    inline void _log(TimestampState&, int);

    inline void _log(PiggybackState*, int);

    void _commit(int, int64_t);

    void _log_inoperation_state();

public:
    State inoperation;

    SharedLockFreeState ();

    ~SharedLockFreeState();

    const char *class_name() const { return "SharedLockFreeState"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    void process_piggyback_message(Packet*);

    void construct_piggyback_message(Packet*);
};

CLICK_ENDDECLS

