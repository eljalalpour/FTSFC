#include <click/config.h>
#include <click/args.hh>
#include <algorithm>
#include <cstring>
#include "ftmb_shared_state.hh"
#include <click/glue.hh>

CLICK_DECLS

FTMBSharedState::FTMBSharedState() { };

FTMBSharedState::~FTMBSharedState() { };

int FTMBSharedState::configure(Vector <String> &conf, ErrorHandler *errh) {
    // set id and f params
    if (Args(conf, this, errh)
                .read("QUEUES", _queues)
                .consume() < 0)
        return -1;

    LOG("FTMB Shared state, queues: %d", _queues);

    //TODO: initialize packets for PALs, for VOR to be sent,


    auto result = SharedStateBase::configure(conf, errh);
    if (result != -1) {

    }//if

    return result;
}

Packet *FTMBSharedState::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(SharedStateBase)
EXPORT_ELEMENT(FTMBSharedState)
