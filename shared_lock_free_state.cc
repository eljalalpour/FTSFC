#include <click/config.h>
#include "shared_lock_free_state.hh"
#include <click/handlercall.hh>

CLICK_DECLS

SharedLockFreeState::SharedLockFreeState () {
    // Initialize the memory of log entries
    for (auto i = 0; i < MB_LEN; ++i) {
        _log_table[i].reserve(LOG_ENTRY_INIT_SIZE);
    }//for
};

SharedLockFreeState::~SharedLockFreeState() { };

int SharedLockFreeState::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("ID", _index)
                .read("F", _failure_count)
                .complete() < 0)
        return -1;
    DEBUG("NFLockFreeCounter index is %d!\n", _index);

    return 0;
}

Packet *SharedLockFreeState::simple_action(Packet *p) {
    return p;
}

void SharedLockFreeState::_log(PiggybackState* p_state, int mb_id) {

}

void SharedLockFreeState::process_piggyback_message(Packet* p) {
    PiggybackMessage* _msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

    for (auto mb_id = 0; mb_id < MB_LEN; ++mb_id) {
        auto last_commit = (mb_id == _id) ? CURRENT_TIMESTAMP : _msg[mb_id]->last_commit;

        // If ack is less than or equal to f, that piggybacked state is the secondary state
        if (_msg[mb_id]->ack <= _failureCount) {
            _log(_msg[mb_id], mb_id);
        }//if
        commit(mb_id, last_commit);
    }//for
}

void SharedLockFreeState::construct_piggyback_message(Packet* p) {

}

inline void SharedLockFreeState::log_in_operation_state() {

}

CLICK_ENDDECLS
EXPORT_ELEMENT(SharedLockFreeState)
