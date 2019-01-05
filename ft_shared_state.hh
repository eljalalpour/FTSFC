#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include "shared_state_base.hh"

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

class FTSharedState : public SharedStateBase {
private:
    int _id;
    int _failure_count;
    int _chain_len;
    std::vector<int> _to_copy_indices;
    int64_t _logic_timestamp;
    State _inoperation;
    int64_t _commit_timestamp;
    Util _util;

#ifdef ENABLE_MULTI_THREADING_USING_MUTEX
    std::mutex _inop_mtx;
    std::mutex _commit_mtx;
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FLAG_LOCK
    flag_spin_lock _inop_fl_lock;
    flag_spin_lock _commit_fl_lock;
#endif

#ifdef ENABLE_MULTI_THREADING_USING_ELIDED_LOCK
    elided_spin_lock _inop_e_lock;
    elided_spin_lock _commit_e_lock;
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FINE_GRAINED_LOCKS
    int64_t _commit_timestamps[STATE_LEN];
    std::mutex _inop_mtxes[STATE_LEN];
    std::mutex _commit_mtxes[STATE_LEN];
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FINE_GRAINED_ELIDED_LOCKS
    int64_t _commit_timestamps[STATE_LEN];
    elided_spin_lock _inop_e_locks[STATE_LEN];
    elided_spin_lock _commit_e_locks[STATE_LEN];
#endif

inline void _capture_inoperation_state(Packet *, int=0);

public:

    FTSharedState ();

    ~FTSharedState();

    const char *class_name() const { return "FTSharedState"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    void process_piggyback_message(Packet*, PiggybackMessage&, int);

    void construct_piggyback_message(Packet*, int=0);

    inline int read(int index) {
#ifdef ENABLE_MULTI_THREADING_USING_MUTEX
        std::lock_guard<std::mutex> lock(_inop_mtx);
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FLAG_LOCK
        elided_lock<flag_spin_lock> flock(_inop_fl_lock);
#endif

#ifdef ENABLE_MULTI_THREADING_USING_ELIDED_LOCK
        elided_lock<elided_spin_lock> elock(_inop_e_lock);
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FINE_GRAINED_LOCKS
        std::lock_guard<std::mutex> lock(_inop_mtxes[index]);
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FINE_GRAINED_ELIDED_LOCKS
        elided_lock<elided_spin_lock> elock(_inop_e_locks[index]);
#endif
        return _inoperation[index];
    }

    inline void increment(int index) {
#ifdef ENABLE_MULTI_THREADING_USING_MUTEX
        std::lock_guard<std::mutex> lock(_inop_mtx);
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FLAG_LOCK
        elided_lock<flag_spin_lock> flock(_inop_fl_lock);
#endif

#ifdef ENABLE_MULTI_THREADING_USING_ELIDED_LOCK
        elided_lock<elided_spin_lock> elock(_inop_e_lock);
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FINE_GRAINED_LOCKS
        std::lock_guard<std::mutex> lock(_inop_mtxes[index]);
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FINE_GRAINED_ELIDED_LOCKS
        elided_lock<elided_spin_lock> elock(_inop_e_locks[index]);
#endif
        ++_inoperation[index];
    }

};

CLICK_ENDDECLS