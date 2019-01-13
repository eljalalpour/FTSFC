#include <click/config.h>
#include <click/args.hh>
#include <algorithm>
#include <cstring>
#include "ft_shared_state.hh"
#include <click/glue.hh>

CLICK_DECLS

FTSharedState::FTSharedState() {
    _util.init(_inoperation);

#ifdef ENABLE_SINGLE_LOCK
    _commit_timestamp = 0;
#endif

#ifdef ENABLE_MULTI_LOCKS
    std::memset(_commit_timestamps, 0, sizeof(_commit_timestamps));
#endif
};

FTSharedState::~FTSharedState() { };

void FTSharedState::process_piggyback_message(Packet *p, PiggybackMessage& log_table, int queue) {
    auto msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

    // Processing the secondary state set
    //COPY_PIGGYBACK_MESSAGE(&log_table, msg);
    for (int i = 0; i < _failure_count; ++i) {
        log_table[_to_copy_indices[i]] = (*msg[_to_copy_indices[i]]);
    }//for

    Locker* locker = get_locker((size_t)queue, Operation::ProcessPiggybackMessage);
    LOCK(locker);
    auto cmt_ts = commit_timestamp(queue);
    if (cmt_ts < msg[_id]->timestamp) {
        set_commit_timestamp(queue, msg[_id]->timestamp);
        UNLOCK(locker);

        cmt_ts = msg[_id]->timestamp;
    }//if
    else {
        UNLOCK(locker);
    }//else

    // This part must be done in construct piggyback message, but we perform it here for performance reasons
    msg[_id]->last_commit = cmt_ts;
}

void FTSharedState::capture_inoperation_state(Packet *p, int queue) {
    DEBUG("Capture _inoperation state");
    PiggybackMessage *msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

//    Locker* locker = get_locker((size_t)queue, Operation::CaptureInOperationState);
//    LOCK(locker);
    _util.copy(msg[_id]->state, _inoperation);
//    UNLOCK(locker);

    msg[_id]->timestamp = click_jiffies();
}

void FTSharedState::construct_piggyback_message(Packet *p, int queue) {
    DEBUG("Construct piggyback message");

    // The secondary state list and that of other middleboxes are already properly updated in
    // process_piggyback_message
    capture_inoperation_state(p, queue);
}

int FTSharedState::configure(Vector <String> &conf, ErrorHandler *errh) {
    // set id and f params
    if (Args(conf, this, errh)
                .read("CHAIN", _chain_len)
                .read("ID", _id)
                .read("F", _failure_count)
                .consume() < 0)
        return -1;

    LOG("Shared state, chain-length: %d, ID: %d, F: %d", _chain_len, _id, _failure_count);
    LOG("To copy indices are: ");
    for (int i = 0; i < _failure_count; ++i) {
        int index = (_id - 1 - i + _chain_len) % _chain_len;
        _to_copy_indices.push_back(index);
        LOG("%d", index);
    }//for

    return SharedStateBase::configure(conf, errh);
}

Packet *FTSharedState::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(SharedStateBase)
EXPORT_ELEMENT(FTSharedState)
