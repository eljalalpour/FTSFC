#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include "ftmb_shared_state.hh"

CLICK_DECLS

class FTMBSharedStateBeamer : public FTMBSharedState {
public:
    FTMBSharedStateBeamer ();

    ~FTMBSharedStateBeamer();

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    const char *class_name() const { return "FTMBSharedStateBeamer"; }

protected:
    virtual inline Locker* get_locker(SharedVarID, int16_t, FTMBOperation);
};

Locker* FTMBSharedStateBeamer::get_locker(SharedVarID var_id, int16_t queue, FTMBOperation op) {
    // Since there is no state sharing, no locking is required!
    return nullptr;
}

CLICK_ENDDECLS
