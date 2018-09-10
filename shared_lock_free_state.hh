#ifndef ATOMIC_ARRAY_HH
#define ATOMIC_ARRAY_HH

#include <click/config.h>
#include <click/element.hh>
#include "defs.hh"

CLICK_DECLS

///
/// ShareLockFreeState maintains the state that is shared among multiple Replica class instances,
/// and performs low-level state management operations.
///

#define LOG_ENTRY_INIT_SIZE 10000

class SharedLockFreeState : public Element {
private:
    LogTable _log_table;
    CommitMemory _commit_memory;

    inline void _log(PiggybackState*, int);

public:
    int in_operation[STATE];

    SharedLockFreeState ();

    ~SharedLockFreeState();

    const char *class_name() const { return "SharedLockFreeState"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *);

    int configure(Vector<String> &conf, ErrorHandler *errh);

    void add_handlers();

    void process_piggyback_message(Packet*);

    void construct_piggyback_message(Packet*);

    void log_in_operation_state();

    inline void process_primary_state(PiggybackMessage*);

    inline void process_seondary_state(PiggybackMessage*);

    void log_in_operation_state();
};

CLICK_ENDDECLS
#endif
