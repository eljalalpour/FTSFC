#include <click/config.h>
#include <click/args.hh>
#include "shared_lock_free_state.hh"

CLICK_DECLS

SharedLockFreeState::SharedLockFreeState () : _log_table() {
    _util.init(inoperation);
    _util.init(_commit_memory);
};

SharedLockFreeState::~SharedLockFreeState() { };

void SharedLockFreeState::_log(State& state, int64_t timestamp, int mb_id) {
    DEBUG("Log operation!");

    auto it = _log_table[mb_id].rbegin();
    if (_log_table[mb_id].empty() ||
        it->timestamp < timestamp) {

        TimestampState t_state;
        _util.copy(t_state.state, state);
        t_state.timestamp = timestamp;
        _log_table[mb_id].push_back(t_state);
    }//if
}

void SharedLockFreeState::_log(TimestampState& t_state, int mb_id) {
//    // Guard the log of a replica
//    std::lock_guard<std::mutex> guard(_log_mutex[mb_id]);

    _log(t_state.state, t_state.timestamp, mb_id);
}

void SharedLockFreeState::_log(PiggybackState& p_state, int mb_id) {
    _log(p_state.state, p_state.timestamp, mb_id);
}

void SharedLockFreeState::_commit(int mb_id, int64_t timestamp) {
//    std::lock_guard<std::mutex> guard(_log_mutex[mb_id]);
    DEBUG("The size of the log table of MB %d: %d", mb_id, _log_table[mb_id].size());

    if (_log_table[mb_id].empty()) {
        return;
    }//

//    auto it = _log_table[mb_id].rbegin();
//    for (; it != _log_table[mb_id].rend(); ++it) {
//        if (timestamp >= it->timestamp) {
//            break;
//        }//if
//    }//for
//
//    if (it != _log_table[mb_id].rend()) {
//        // Commit involves storing the most updated log value into commit memory, setting the timestamp time, and
//        // erasing committed logs.
//        _util.copy(_commit_memory[mb_id].state, it->state);
//        _commit_memory[mb_id].timestamp = timestamp;
//
//        // TODO: make sure it works!!
//        _log_table[mb_id].erase(_log_table[mb_id].begin(), std::next(it).base());
//    }//if

    int i = (int) (_log_table[mb_id].size()) - 1;
    for (; i >= 0; --i) {
        if (timestamp >= _log_table[mb_id][i].timestamp) {
            break;
        }//if
    }//for

    if (i != -1) {
        // Commit involves storing the most updated log value into commit memory, setting the timestamp time, and
        // erasing committed logs.
        _util.copy(_commit_memory[mb_id].state, _log_table[mb_id][i].state);
        _commit_memory[mb_id].timestamp = timestamp;

        // TODO: make sure it works!!
        _log_table[mb_id].erase((size_t)(i + 1));
    }//if
}

void SharedLockFreeState::process_piggyback_message(Packet* p) {
    auto msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

    // Processing the primary state:
    // TODO: Check if this is correct? In the paper, we tell it must be msg[mb_id].timestamp
//    commit(_id, CURRENT_TIMESTAMP);
    _commit(_id, msg[_id]->timestamp);

    // Processing the secondary state set
    for (int i = 1; i <= _failure_count; ++i) {
        int mb_id = (_id - i + _chain_len) % _chain_len;

        _commit(mb_id, msg[mb_id]->last_commit);

        _log(*msg[mb_id], mb_id);

        // The following part is must be placed in construct_piggyback_message,
        // but to make it faster, I perform it here:
        // PART START
        ++msg[mb_id]->ack;
        // PART END
    }//for

//    LOG("After processing piggyback message:");
//    _util.print(*msg[0]);
//    _util.print(*msg[1]);
}

void SharedLockFreeState::_log_inoperation_state() {
    DEBUG("Log inoperation state");

    TimestampState ts;
    ts.timestamp = CURRENT_TIMESTAMP;
    _util.copy(ts.state, inoperation);

    // TODO: remove this part
//    if(_log_table[_id].size() < LOG_TABLE_CON_SIZE) {
//        _log_table[_id].push_back(ts);
//    }//if

//    // TODO: remove this part
    if (!_log_table[_id].full()) {
        _log_table[_id].push_back(ts);
    }//if
//    else {
//        LOG("Full log table!");
//    }//else
}

void SharedLockFreeState::construct_piggyback_message(Packet* p) {
    DEBUG("Construct piggyback message");

    PiggybackMessage* msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

    // The secondary state list and that of other middleboxes are already properly updated in
    // process_piggyback_message

    // Since the state is small, put the whole state into the packet
    _log_inoperation_state();

    auto it = _log_table[_id].rbegin();
    _util.copy(msg[_id]->state, it->state);
    msg[_id]->timestamp = it->timestamp;
    msg[_id]->last_commit = _commit_memory[_id].timestamp;
    msg[_id]->ack = 1;

//    LOG("After construct piggyback message:");
//    _util.print(*msg[0]);
//    _util.print(*msg[1]);
}

int SharedLockFreeState::configure(Vector<String> &conf, ErrorHandler *errh) {
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

size_t SharedLockFreeState::log_table_length() {
    return this->_log_table->size();
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SharedLockFreeState)
