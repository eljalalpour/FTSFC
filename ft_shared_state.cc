#include <click/config.h>
#include <click/args.hh>
#include <algorithm>
#include <cstring>
#include "ft_shared_state.hh"
#include <click/glue.hh>

CLICK_DECLS

FTSharedState::FTSharedState() {
    _util.init(_inoperation);
    _commit_timestamp = 0;
    _logic_timestamp = _commit_timestamp + 1;

#ifdef ENABLE_MULTI_THREADING_USING_FINE_GRAINED_LOCKS
    std::memset(_commit_timestamps, 0, sizeof(_commit_timestamps));
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FINE_GRAINED_ELIDED_LOCKS
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

    if (_commit_timestamp < msg[_id]->timestamp) {

#ifdef ENABLE_MULTI_THREADING_USING_MUTEX
        std::lock_guard<std::mutex> lock(_commit_mtx);
#endif
#ifdef ENABLE_MULTI_THREADING_USING_FLAG_LOCK
        elided_lock<flag_spin_lock> flock(_commit_fl_lock);
#endif
#ifdef ENABLE_MULTI_THREADING_USING_ELIDED_LOCK
        elided_lock<elided_spin_lock> elock(_commit_e_lock);
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FINE_GRAINED_LOCKS
        std::lock_guard<std::mutex> lock(_commit_mtxes[queue]);
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FINE_GRAINED_ELIDED_LOCKS
        elided_lock<elided_spin_lock> elock(_commit_e_locks[queue]);
#endif
        _commit_timestamp = msg[_id]->timestamp;
        // This part must be done in construct piggyback message, but we perform it here for performance reasons
        msg[_id]->last_commit = _commit_timestamp;
    }//if
    else {
        // This part must be done in construct piggyback message, but we perform it here for performance reasons
        // Note the following line repeats the same line in 'if' block, since to be lock free for the cases that
        //_commit_timestamp is only read.
        msg[_id]->last_commit = _commit_timestamp;
    }//else
}

void FTSharedState::_capture_inoperation_state(Packet *p, int queue) {
    DEBUG("Capture _inoperation state");
    PiggybackMessage *msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

    {
#ifdef ENABLE_MULTI_THREADING_USING_MUTEX
        std::lock_guard<std::mutex> lock(_inop_mtx);
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FLAG_LOCK
        elided_lock<flag_spin_lock> lock(_inop_fl_lock);
#endif

#ifdef ENABLE_MULTI_THREADING_USING_ELIDED_LOCK
        elided_lock<elided_spin_lock> elock(_inop_e_lock);
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FINE_GRAINED_LOCKS
        std::lock_guard<std::mutex> lock(_inop_mtxes[queue]);
#endif
#ifdef ENABLE_MULTI_THREADING_USING_FINE_GRAINED_ELIDED_LOCKS
        elided_lock<elided_spin_lock> elock(_inop_e_locks[queue]);
#endif
        _util.copy(msg[_id]->state, _inoperation);
    }

    //TODO: check the performance of the atomic version
//    msg[_id]->timestamp = CURRENT_TIMESTAMP;
    msg[_id]->timestamp = ++_logic_timestamp;
}

void FTSharedState::construct_piggyback_message(Packet *p, int queue_id) {
    DEBUG("Construct piggyback message");

    // The secondary state list and that of other middleboxes are already properly updated in
    // process_piggyback_message
    _capture_inoperation_state(p, queue_id);
}

int FTSharedState::configure(Vector <String> &conf, ErrorHandler *errh) {
    // set id and f params
    if (Args(conf, this, errh)
                .read("CHAIN", _chain_len)
                .read("ID", _id)
                .read("F", _failure_count)
                .complete() < 0)
        return -1;

    LOG("Shared state, chain-length: %d, ID: %d, F: %d", _chain_len, _id, _failure_count);
    LOG("To copy indices are: ");
    for (int i = 0; i < _failure_count; ++i) {
        int index = (_id - 1 - i + _chain_len) % _chain_len;
        _to_copy_indices.push_back(index);
        LOG("%d", index);
    }//for

    return 0;
}

Packet *FTSharedState::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SharedLockFreeState)
