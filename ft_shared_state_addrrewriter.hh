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

    void register_addr_rewriter(int, uint32_t*);

protected:
    inline uint32_t _lock_index(size_t index);
    virtual inline Locker* get_locker(size_t, Operation);

private:
    uint32_t* _reg_last_locks[MAX_QUEUES];
    uint32_t _registered;
};

uint32_t FTSharedStateAddrRewriter::_lock_index(size_t index) {
    return *_reg_last_locks[index];
}

Locker* FTSharedStateAddrRewriter::get_locker(size_t index_or_queue, Operation op) {

    if (_registered <= 1)
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