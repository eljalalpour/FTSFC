#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include "ft_shared_state.hh"

CLICK_DECLS

class FTSharedStateCounter : public FTSharedState {
public:
    const char *class_name() const { return "FTSharedStateCounter"; }

protected:
    virtual inline Locker* get_locker(size_t, Operation);

    inline size_t _lock_index(size_t index);
};

size_t FTSharedStateCounter::_lock_index(size_t index) {
    return index % (STATE_LEN / _sharing_level);
}

Locker* FTSharedStateCounter::get_locker(size_t index_or_queue, Operation op) {
    Locker* locker = nullptr;

    switch (op) {
        case Operation::CaptureInOperationState:
        case Operation::Increment:
        case Operation::Read:
            auto lock_index = _lock_index(index_or_queue);
            locker = &_shared_locks->locker_ptr(lock_index);
            break;

        default:
            locker = FTSharedState::get_locker(index_or_queue, op);
            break;
    }//switch

    return locker;
}

CLICK_ENDDECLS