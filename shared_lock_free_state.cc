#include <click/config.h>
#include <click/args.hh>
#include "shared_lock_free_state.hh"

CLICK_DECLS

SharedLockFreeState::SharedLockFreeState () { };

SharedLockFreeState::~SharedLockFreeState() { };

void SharedLockFreeState::_log(PiggybackState* p_state, int mb_id) {
    // Guard the log of a replica
//    std::lock_guard<std::mutex> guard(_log_mutex[mb_id]);

    TimestampState ts_state;
    _util.copy(ts_state, *p_state);
    _log_table[mb_id].push_back(ts_state);
}

void SharedLockFreeState::_commit(int mb_id, int64_t timestamp) {
    DEBUG("Commiting");
//    std::lock_guard<std::mutex> guard(_log_mutex[mb_id]);

    if (_log_table[mb_id].empty()) {
        return;
    }//

    DEBUG("Log table");
    auto it = _log_table[mb_id].rbegin();
    for (; it != _log_table[mb_id].rend(); ++it) {
        DEBUG("In for");
        DEBUG("Log table size: %d", _log_table[mb_id].size());
        if (timestamp >= it->timestamp) {
            DEBUG("In if");
            break;
        }//if
        DEBUG("After if");
    }//for

    DEBUG("check if there is some thing to commit");
    if (it != _log_table[mb_id].rend()) {
        // Commit involves storing the most updated log value into commit memory, setting the timestamp time, and
        // erasing committed logs.
        DEBUG("copy commit memory using util");
        _util.copy(_commit_memory[mb_id].state, it->state);

        DEBUG("commit memory");
        _commit_memory[mb_id].timestamp = timestamp;

        // TODO: make sure it works!!
        DEBUG("erase from log table");
        _log_table[mb_id].erase(_log_table[mb_id].begin(), std::next(it).base());
    }//if
}

void SharedLockFreeState::process_piggyback_message(Packet* p) {
    DEBUG("Casting!");
    PiggybackMessage* _msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

    DEBUG("Message:");
    _util.print(*_msg);

    // Processing the primary state:
    // TODO: Check if this is correct? In the paper, we tell it must be _msg[mb_id].timestamp
//    commit(_id, CURRENT_TIMESTAMP);
    _commit(_id, _msg[_id]->timestamp);

    // Processing the secondary state set
    for (auto i = 1; i <= _failure_count; ++i) {
        auto mb_id = (_id - i) % MB_LEN;

        // The following part is must be placed in construct_piggyback_message,
        // but to make it faster, I perform it here:

        // PART START
        ++_msg[mb_id]->ack;
        // PART END

        _commit(mb_id, _msg[mb_id]->last_commit);
        _log(_msg[mb_id], mb_id);
    }//for
}

void SharedLockFreeState::log_inoperation_state() {
    TimestampState ts;
    ts.timestamp = CURRENT_TIMESTAMP;
    _util.copy(ts.state, inoperation);

    _log_table[_id].push_back(ts);
}

void SharedLockFreeState::construct_piggyback_message(Packet* p) {
    PiggybackMessage* msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

    // The secondary state list and that of other middleboxes are already properly updated in
    // process_piggyback_message

    // Since the state is small, put the whole state into the packet
    auto it = _log_table[_id].rbegin();
    _util.copy(msg[_id]->state, it->state);
    msg[_id]->timestamp = it->timestamp;
    msg[_id]->last_commit = _commit_memory[_id].timestamp;
    msg[_id]->ack = 1;
}

int SharedLockFreeState::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set id and f params
    if (Args(conf, this, errh)
                .read("ID", _id)
                .read("F", _failure_count)
                .complete() < 0)
        return -1;

    DEBUG("SharedLockFreeState is %d!\n", _id);

    return 0;
}

Packet *SharedLockFreeState::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SharedLockFreeState)
