#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include "ft_shared_state.hh"
#include "ft_addrrewriter.hh"

CLICK_DECLS

class FTSharedStateAddrRewriter : public FTSharedState {
public:
    FTSharedStateAddrRewriter ();

    ~FTSharedStateAddrRewriter();

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    const char *class_name() const { return "FTSharedStateAddrRewriter"; }

    void register_addr_rewriter(int, FTAddrRewriter*);

protected:
    inline size_t _lock_index(size_t index);
    virtual inline Locker* get_locker(size_t, Operation);

private:
    FTAddrRewriter* _reg_addr_rw[MAX_QUEUES];
};

size_t FTSharedStateAddrRewriter::_lock_index(size_t index) {
    return _reg_addr_rw[index]->last_lock_index;
}

Locker* FTSharedStateAddrRewriter::get_locker(size_t index_or_queue, Operation op) {
    if (_sharing_level == ThreadSharing1)
        return nullptr;

    Locker* locker = nullptr;

    switch (op) {
        default:
            locker = FTSharedState::get_locker(index_or_queue, op);
            break;

        case Operation::CaptureInOperationState:
        case Operation::Increment:
        case Operation::Read:
            locker = _shared_locks->locker_ptr(
                                            _lock_index(
                                                    index_or_queue));
            break;
    }//switch

    return locker;
}

CLICK_ENDDECLS