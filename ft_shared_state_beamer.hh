#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include "ft_shared_state.hh"

CLICK_DECLS

class FTSharedStateBeamer : public FTSharedState {
public:
    FTSharedStateBeamer ();

    ~FTSharedStateBeamer();

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    const char *class_name() const { return "FTSharedStateBeamer"; }

protected:
    virtual inline Locker* get_locker(size_t, Operation);
};

Locker* FTSharedStateBeamer::get_locker(size_t index_or_queue, Operation op) {
    // Since there is no state sharing, no locking is required!
    return nullptr;
}

CLICK_ENDDECLS