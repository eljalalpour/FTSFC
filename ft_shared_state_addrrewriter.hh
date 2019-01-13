#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include <click/hashcontainer.hh>
#include "ft_shared_state.hh"

CLICK_DECLS

class FTSharedStateAddrRewriter : public FTSharedState {
public:
    FTSharedStateAddrRewriter ();

    ~FTSharedStateAddrRewriter();

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    const char *class_name() const { return "FTSharedStateAddrRewriter"; }

    virtual inline Locker* get_locker(size_t);

protected:
    inline uint32_t _lock_index(size_t index);
    virtual inline Locker* get_locker(size_t, Operation);
};

uint32_t FTSharedStateAddrRewriter::_lock_index(size_t index) {
    return *_reg_last_locks[index];
}

Locker* FTSharedStateAddrRewriter::get_locker(size_t index) {
    return get_locker(index, Operation::PacketTransaction);
}

Locker* FTSharedStateAddrRewriter::get_locker(size_t index_or_queue, Operation op) {

    if (_registered <= 1)
        return nullptr;

    Locker* locker = nullptr;

    switch (op) {
        default:
            locker = FTSharedState::get_locker(index_or_queue, op);
            break;

        case PacketTransaction:
            locker = _shared_locks->locker_ptr(index_or_queue);
            break;
    }//switch

    return locker;
}

CLICK_ENDDECLS