#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include "ftmb_shared_state.hh"

CLICK_DECLS

class FTMBSharedStateCounter : public FTMBSharedState {
public:
    enum {
        ThreadSharing1 = 1, // no sharing
        ThreadSharing2 = 2, // 2 threads share the same counter
        ThreadSharing4 = 4, // 4 threads share the same counter
        ThreadSharing8 = 8, // all threads share the same counter
    };

    FTMBSharedStateCounter ();

    ~FTMBSharedStateCounter();

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    const char *class_name() const { return "FTMBSharedStateCounter"; }

protected:
    int _sharing_level;

    inline int16_t _lock_index(int16_t index);
    virtual inline Locker* get_locker(int16_t, int16_t, FTMBOperation);
};

int16_t FTMBSharedStateCounter::_lock_index(int16_t index) {
    return index % (_cached_lockers_size / _sharing_level);
}

Locker* FTMBSharedStateCounter::get_locker(int16_t var_id, int16_t queue, FTMBOperation op) {
    if (_sharing_level == ThreadSharing1)
        return nullptr;

    Locker* locker = nullptr;

    switch (op) {
        case FTMBOperation::AccessSharedVariable:
            locker = _shared_locks->locker_ptr(_lock_index(var_id));
            break;
    }//switch

    return locker;
}

CLICK_ENDDECLS
