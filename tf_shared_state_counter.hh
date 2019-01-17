#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "tf_shared_state.hh"

CLICK_DECLS

class TFSharedStateCounter: public TFSharedState {
public:
    enum {
        ThreadSharing1 = 1, // no sharing
        ThreadSharing2 = 2, // 2 threads share the same counter
        ThreadSharing4 = 4, // 4 threads share the same counter
        ThreadSharing8 = 8, // all threads share the same counter
    };

    TFSharedStateCounter ();

    ~TFSharedStateCounter();

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    const char *class_name() const { return "TFSharedStateCounter"; }

protected:
    int _sharing_level;
    inline size_t _lock_index(int16_t index);
    virtual inline Locker* get_locker(int16_t, int16_t, TFOperation = TFOperation::TFAccessSharedVariable);
};

size_t TFSharedStateCounter::_lock_index(int16_t index) {
    auto re = index % (_shared_locks->size() / _sharing_level);
    return re;
}

Locker* TFSharedStateCounter::get_locker(int16_t var_id, int16_t queue, TFOperation op) {
    if (_sharing_level == ThreadSharing1)
        return nullptr;

    Locker* locker = nullptr;

    switch (op) {
        case TFOperation::TFAccessSharedVariable:
            locker = _shared_locks->locker_ptr(_lock_index(var_id));
            break;
    }//switch

    return locker;
}

CLICK_ENDDECLS
