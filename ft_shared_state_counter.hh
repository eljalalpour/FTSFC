#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include "ft_shared_state.hh"

CLICK_DECLS

class FTSharedStateCounter : public FTSharedState {
public:
    enum {
        ThreadSharing1 = 1, // no sharing
        ThreadSharing2 = 2, // 2 threads share the same counter
        ThreadSharing4 = 4, // 4 threads share the same counter
        ThreadSharing8 = 8, // all threads share the same counter
    };

    FTSharedStateCounter ();

    ~FTSharedStateCounter();

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    const char *class_name() const { return "FTSharedStateCounter"; }

    virtual inline Locker* get_locker(size_t);

protected:
    int _sharing_level;
    inline size_t _lock_index(size_t index);
    virtual inline Locker* get_locker(size_t, Operation);
};

size_t FTSharedStateCounter::_lock_index(size_t index) {
    return index % (_cached_lockers_size / _sharing_level);
}

Locker* FTSharedStateCounter::get_locker(size_t index) {
    return get_locker(index, Operation::PacketTransaction);
}

Locker* FTSharedStateCounter::get_locker(size_t index_or_queue, Operation op) {
    if (_sharing_level == ThreadSharing1)
        return nullptr;

    Locker* locker = nullptr;

    switch (op) {
        default:
            locker = FTSharedState::get_locker(index_or_queue, op);
            break;

//        case Operation::CaptureInOperationState:
//        case Operation::Increment:
//        case Operation::Read:
//        case Operation::Increment:
        case Operation::PacketTransaction:
            auto lock_index = _lock_index(index_or_queue);
            locker = _shared_locks->locker_ptr(lock_index);
            break;
    }//switch

    return locker;
}

CLICK_ENDDECLS