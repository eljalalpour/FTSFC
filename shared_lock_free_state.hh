#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include <condition_variable>

CLICK_DECLS

///
/// ShareLockFreeState maintains the state that is shared among multiple Replica class instances,
/// and performs low-level state management operations.
/// \textbf{Assumptions}
/// \begin{itemize}
///     \item We assume that a middlebox during processing a packet modifies the in-operation state only once.
///     \item The above modification is performed using increment function
/// \end{itemize}
///

//#define ENABLE_MULTI_THREADING 1

class SharedLockFreeState : public Element {
private:
    int _id;
    int _failure_count;
    int _chain_len;

    int64_t _logic_timestamp;

    State _inoperation;
    std::mutex _inop_mtx;

    int64_t _commit_timestamp;
    std::mutex _commit_mtx;

    Util _util;

    inline void _capture_inoperation_state(Packet *, int=0);

public:

    SharedLockFreeState ();

    ~SharedLockFreeState();

    const char *class_name() const { return "SharedLockFreeState"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    void process_piggyback_message(Packet*, PiggybackMessage&);

    void construct_piggyback_message(Packet*, int=0);

    inline int read(int index);

    inline void increment(int index);
};

CLICK_ENDDECLS