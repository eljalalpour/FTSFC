#pragma once

#include <click/config.h>
#include <click/element.hh>
#include "shared_state_base.hh"

#define DEFAULT_SIZE         8
#define DEFAULT_HANDLER_SIZE 8

CLICK_DECLS

class SharedState : public SharedStateBase {
public:
    const char *class_name() const { return "SharedState"; }

    inline void increment(size_t);

    inline int read(size_t);
};

void SharedState::increment(size_t index) {
    size_t lock_index = index % (DEFAULT_SIZE / this->_sharing_level);
    _shared_locks->lock(lock_index);
    ++_state[index];
    _shared_locks->unlock(lock_index);
}

int SharedState::read(size_t index) {
    size_t lock_index = index % (DEFAULT_SIZE / _sharing_level);
    _shared_locks->lock(lock_index);
    auto val = _state[index];
    _shared_locks->unlock(lock_index);

    return val;
}

CLICK_ENDDECLS
