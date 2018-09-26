#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>

CLICK_DECLS

///
/// ShareLockFreeState maintains the state that is shared among multiple Replica class instances,
/// and performs low-level state management operations.
/// \textbf{Assumptions}
/// \begin{itemize}
///     \item We assume that a middlebox during processing a packet modifies the in-operation state only once.
///     \item The above modification is performed using increment function
/// \end{itemize}


#define LOG_TABLE_CONTROL 100
#define NOT_MODIFYING     0
#define NOT_CAPTURING     0

class SharedLockFreeState : public Element {
private:
    int _id;
    int _failure_count;
    int _chain_len;

    LogTable _log_table;
    LogTableMutex _log_table_mutex;

    CommitMemory _commit_memory;
    CommitMemoryMutex _commit_memory_mutex;

    State _inoperation;
    volatile size_t _modifying_phase;
    std::condition_variable _modifying_phase_cv;
    std::mutex _modifying_phase_mtx;

    volatile size_t _capture_inop_phase;
    std::condition_variable _capture_inop_phase_cv;
    std::mutex _capture_inop_phase_mtx;

    Util _util;

    inline void _log(State&, int64_t, int);

    inline void _log(TimestampState&, int);

    inline void _log(PiggybackState&, int);

    void _commit(int, int64_t);

    inline void _capture_inoperation_state(State&, int=0);

    void _log_inoperation_state(int=0);

public:

    SharedLockFreeState ();

    ~SharedLockFreeState();

    const char *class_name() const { return "SharedLockFreeState"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    void process_piggyback_message(Packet*);

    void construct_piggyback_message(Packet*, int=0);

    size_t log_table_length();

    inline int read(int&);

    inline void increment(int&);
};

CLICK_ENDDECLS
