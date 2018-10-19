#include <click/config.h>
#include <click/args.hh>
#include <algorithm>
#include "shared_lock_free_state.hh"

CLICK_DECLS

#define ENABLE_MULTI_THREADING 1

SharedLockFreeState::SharedLockFreeState() {
    _util.init(_inoperation);
};

SharedLockFreeState::~SharedLockFreeState() { };

void SharedLockFreeState::process_piggyback_message(Packet *p, PiggybackMessage& log_table) {
    auto msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

    // Processing the secondary state set
    COPY_PIGGYBACK_MESSAGE(log_table, msg);
//    auto end = std::min(_id - 1, _chain_len - 1);
//    auto len = std::min(_failure_count, _chain_len + _failure_count - _id);
//    for (int i = end; i > (end - len) && i >= 0; --i) {
//        log_table[i] = (*msg[i]);
//    }//for
}

void SharedLockFreeState::_capture_inoperation_state(Packet *p, int thread_id) {
    DEBUG("Capture _inoperation state");
    PiggybackMessage *msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

#ifdef ENABLE_MULTI_THREADING
    {
        std::lock_guard<std::mutex> lock(_inop_mtx);
        _util.copy(msg[_id]->state, _inoperation);
    }
    msg[_id]->timestamp = CURRENT_TIMESTAMP;
#else
    _util.copy(msg[_id]->state, _inoperation);
    msg[_id]->timestamp = CURRENT_TIMESTAMP;
#endif
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
    ++_inoperation[index];
#else
    ++_inoperation[index];
#endif
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SharedLockFreeState)