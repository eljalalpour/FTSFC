#include <click/config.h>
#include <click/args.hh>
#include <algorithm>
#include "shared_lock_free_state.hh"

CLICK_DECLS

SharedLockFreeState::SharedLockFreeState() {
    _util.init(_inoperation);
    _commit_timestamp = 0;
    _logic_timestamp = _commit_timestamp + 1;
};

SharedLockFreeState::~SharedLockFreeState() { };

void SharedLockFreeState::process_piggyback_message(Packet *p, PiggybackMessage& log_table) {
    auto msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

    // Processing the secondary state set
    COPY_PIGGYBACK_MESSAGE(&log_table, msg);
//    for (int i = 0; i < _chain_len; ++i) {
//        log_table[i] = (*msg[i]);
//    }//for

    {//
#ifdef ENABLE_MULTI_THREADING
        std::lock_guard<std::mutex> lock(_commit_mtx);
#endif
        if (_commit_timestamp < msg[_id]->timestamp)
            _commit_timestamp = msg[_id]->timestamp;

        // This part must be done in construct piggyback message, but we perform it here for performance reasons
        msg[_id]->last_commit = _commit_timestamp;
    }
}

void SharedLockFreeState::_capture_inoperation_state(Packet *p, int thread_id) {
    DEBUG("Capture _inoperation state");
    PiggybackMessage *msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

    {
#ifdef ENABLE_MULTI_THREADING
        std::lock_guard<std::mutex> lock(_inop_mtx);
#endif
        _util.copy(msg[_id]->state, _inoperation);
    }

//    msg[_id]->timestamp = CURRENT_TIMESTAMP;
    msg[_id]->timestamp = ++_logic_timestamp;
}

void SharedLockFreeState::construct_piggyback_message(Packet *p, int thread_id) {
    DEBUG("Construct piggyback message");

    // The secondary state list and that of other middleboxes are already properly updated in
    // process_piggyback_message
    _capture_inoperation_state(p, thread_id);
}

int SharedLockFreeState::configure(Vector <String> &conf, ErrorHandler *errh) {
    // set id and f params
    if (Args(conf, this, errh)
                .read("CHAIN", _chain_len)
                .read("ID", _id)
                .read("F", _failure_count)
                .complete() < 0)
        return -1;
    LOG("Shared state, chain-length: %d, ID: %d, F: %d", _chain_len, _id, _failure_count);
    return 0;
}

Packet *SharedLockFreeState::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SharedLockFreeState)
