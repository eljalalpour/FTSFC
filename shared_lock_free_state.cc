#include <click/config.h>
#include <click/args.hh>
#include <algorithm>
#include "shared_lock_free_state.hh"

CLICK_DECLS

#define ENABLE_MULTI_THREADING 1

SharedLockFreeState::SharedLockFreeState() {
    _util.init(_inoperation);
    _commit_timestamp = 0;
};

SharedLockFreeState::~SharedLockFreeState() { };

void SharedLockFreeState::process_piggyback_message(Packet *p, PiggybackMessage& log_table) {
    auto msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

    // Processing the secondary state set
    COPY_PIGGYBACK_MESSAGE(log_table, msg);

    {//
#ifdef ENABLE_MULTI_THREADING
        std::lock_guard<std::mutex> lock(_commit_mtx);
#endif
        if (_commit_timestamp < log_table[_id].timestamp)
            _commit_timestamp = log_table[_id].timestamp;

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

    msg[_id]->timestamp = CURRENT_TIMESTAMP;
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

    return 0;
}

Packet *SharedLockFreeState::simple_action(Packet *p) {
    return p;
}

int SharedLockFreeState::read(int index) {
    return _inoperation[index];
}

void SharedLockFreeState::increment(int index) {
#ifdef ENABLE_MULTI_THREADING
    std::lock_guard<std::mutex> lock(_inop_mtx);
#endif
    ++_inoperation[index];
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SharedLockFreeState)