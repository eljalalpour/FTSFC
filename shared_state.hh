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

private:
    int _sharing_level;
    inline size_t _lock_index(size_t index);
};

size_t SharedState::_lock_index(size_t index) {
    return index % (STATE_LEN / _sharing_level);
}

void SharedState::increment(size_t index) {
    auto lock_index = _lock_index(index);
    _shared_locks->lock(lock_index);
    ++_state[index];
    _shared_locks->unlock(lock_index);
}

int SharedState::read(size_t index) {
    auto lock_index = _lock_index(index);
    _shared_locks->lock(lock_index);
    auto val = _state[index];
    _shared_locks->unlock(lock_index);

    return val;
}

CLICK_ENDDECLS
