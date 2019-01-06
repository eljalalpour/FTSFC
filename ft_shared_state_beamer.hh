#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include "ft_shared_state.hh"

CLICK_DECLS

class FTSharedStateBeamer : public FTSharedState {
public:
    enum {
        ThreadSharing1 = 1, // no sharing
        ThreadSharing2 = 2, // 2 threads share the same counter
        ThreadSharing4 = 4, // 4 threads share the same counter
        ThreadSharing8 = 8, // all threads share the same counter
    };

    FTSharedStateBeamer ();

    ~FTSharedStateBeamer();

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    const char *class_name() const { return "FTSharedStateBeamer"; }

protected:
    inline size_t _lock_index(size_t index);
    virtual inline Locker* get_locker(size_t, Operation);
};

size_t FTSharedStateBeamer::_lock_index(size_t index) {
    return index % (_cached_lockers_size / _sharing_level);
}

Locker* FTSharedStateBeamer::get_locker(size_t index_or_queue, Operation op) {
    // Since there is no state sharing, no locking is required!
    return nullptr;
}

CLICK_ENDDECLS