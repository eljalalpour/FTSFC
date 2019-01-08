#pragma once

#include <click/config.h>
#include <click/element.hh>
#include "shared_state_base.hh"

CLICK_DECLS

class SharedState : public SharedStateBase {
public:
    enum {
        ThreadSharing1 = 1, // no sharing
        ThreadSharing2 = 2, // 2 threads share the same counter
        ThreadSharing4 = 4, // 4 threads share the same counter
        ThreadSharing8 = 8, // all threads share the same counter
    };

    const char *class_name() const { return "SharedState"; }

    int configure(Vector<String>&, ErrorHandler*) CLICK_COLD;

    inline void increment(size_t);

    inline int read(size_t);

protected:
    int _sharing_level;
    inline size_t _lock_index(size_t index);

private:
    virtual inline Locker* get_locker(size_t);
};

size_t SharedState::_lock_index(size_t index) {
    return index % (_cached_lockers_size / _sharing_level);
}

Locker* SharedState::get_locker(size_t index_or_queue) {
    if (_sharing_level == ThreadSharing1)
        return nullptr;

    auto lock_index = _lock_index(index_or_queue);
    return _shared_locks->locker_ptr(lock_index);
}

void SharedState::increment(size_t index) {
    auto locker = get_locker(index);
    LOCK(locker);
    ++_state[index];
    UNLOCK(locker);
}

int SharedState::read(size_t index) {
    auto locker = get_locker(index);
    LOCK(locker);
    auto val = _state[index];
    UNLOCK(locker);

    return val;
}

CLICK_ENDDECLS
