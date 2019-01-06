#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include "shared_state_base.hh"

CLICK_DECLS

///
/// FTSharedState maintains the state that is shared among multiple Replica class instances,
/// and performs low-level state management operations.
/// \textbf{Assumptions}
/// \begin{itemize}
///     \item We assume that a middlebox during processing a packet modifies the in-operation state only once.
///     \item The above modification is performed using increment function
/// \end{itemize}
///

#define ENABLE_SINGLE_LOCK
#define ENABLE_MULTI_LOCKS

enum Operation {
    ProcessPiggybackMessage,
    CaptureInOperationState,
    PacketTransaction,
    Increment,
    Read
};

class FTSharedState : public SharedStateBase {
private:
    int _id;
    int _failure_count;
    int _chain_len;
    std::vector<int> _to_copy_indices;
    State _inoperation;
    Util _util;

#ifdef ENABLE_SINGLE_LOCK
    int64_t _commit_timestamp;
    Locker _inop_mtx;
    Locker _commit_mtx;
#endif

#ifdef ENABLE_MULTI_LOCKS
    int64_t _commit_timestamps[STATE_LEN];
    std::mutex _inop_mtxes[STATE_LEN];
    std::mutex _commit_mtxes[STATE_LEN];
#endif

inline void _capture_inoperation_state(Packet *, int=0);

public:
    FTSharedState ();

    ~FTSharedState();

    const char *class_name() const { return "FTSharedState"; }

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    void process_piggyback_message(Packet*, PiggybackMessage&, int);

    void construct_piggyback_message(Packet*, int=0);

    inline int64_t commit_timestamp(int);

    inline void set_commit_timestamp(int, int64_t);

    inline int read(size_t);

    inline void increment(size_t);

protected:
    virtual inline Locker* get_locker(size_t, Operation);
};


int64_t FTSharedState::commit_timestamp(int queue) {
#ifdef ENABLE_SINGLE_LOCK
    return _commit_timestamp;
#endif
#ifdef ENABLE_MULTI_LOCKS
    return _commit_timestamps[queue];
#else
    return 0;
#endif
}

void FTSharedState::set_commit_timestamp(int queue, int64_t val) {
#ifdef ENABLE_SINGLE_LOCK
    _commit_timestamp = val;
#endif
#ifdef ENABLE_MULTI_LOCKS
    _commit_timestamps[queue] = val;
#endif
}

int FTSharedState::read(size_t index) {
    Locker* locker = get_locker(index, Operation::Read);
    LOCK(locker);
    auto val = _inoperation[index];
    UNLOCK(locker);

    return val;
}

void FTSharedState::increment(size_t index) {
    Locker* locker = get_locker(index, Operation::Increment);

    LOCK(locker);
    ++_inoperation[index];
    UNLOCK(locker);
}


Locker* FTSharedState::get_locker(size_t index_or_queue, Operation op) {
    Locker* locker = nullptr;

#ifdef ENABLE_SINGLE_LOCK
    switch (op) {
        case Operation::ProcessPiggybackMessage:
            locker = &_commit_mtx;
            break;

        case Operation::CaptureInOperationState:
        case Operation::Increment:
        case Operation::Read:
            locker = &_inop_mtx;
            break;
    }//switch
#endif

#ifdef ENABLE_MULTI_LOCKS
    switch (op) {
        case Operation::ProcessPiggybackMessage:
            locker = &_commit_mtxes[index_or_queue];
            break;

        case Operation::CaptureInOperationState:
        case Operation::Increment:
        case Operation::Read:
            locker = &_inop_mtxes[index_or_queue];
            break;
    }//switch
#endif

    return locker;
}

CLICK_ENDDECLS