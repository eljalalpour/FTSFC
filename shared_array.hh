#pragma once

#include <click/config.h>
#include <click/element.hh>
#include "shared_array_base.hh"

#define DEFAULT_SIZE         8
#define DEFAULT_HANDLER_SIZE 8

CLICK_DECLS

class SharedArray : public SharedArrayBase {
public:
    inline void increment(size_t);

    inline int read(size_t);
};

void SharedArray::increment(size_t index) {
    size_t lock_index = index % (DEFAULT_SIZE / this->_sharing_level);
    _shared_locks->lock(lock_index);
    ++_array[index];
    _shared_locks->unlock(lock_index);
}

int SharedArray::read(size_t index) {
    size_t lock_index = index % (DEFAULT_SIZE / _sharing_level);
    _shared_locks->lock(lock_index);
    auto val = _array[index];
    _shared_locks->unlock(lock_index);

    return val;
}

CLICK_ENDDECLS
