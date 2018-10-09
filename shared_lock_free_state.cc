#include <click/config.h>
#include <click/args.hh>
#include "shared_lock_free_state.hh"

CLICK_DECLS

#define ENABLE_MULTI_THREADING 1

SharedLockFreeState::SharedLockFreeState() {
    _util.init(_inoperation);

    // No thread at the beginning is modifying in-operation state or
    // is capturing the inoperation state
    _modifying_phase = 0;
    _capture_inop_phase = 0;
};

SharedLockFreeState::~SharedLockFreeState() { };

void SharedLockFreeState::_log_secondary_state(State &state, int64_t timestamp, int mb_id, LogTable& log_table) {
    auto it = log_table[mb_id].rbegin();
    if (log_table[mb_id].empty() ||
        it->timestamp < timestamp) {
        log_table[mb_id].emplace_back(timestamp, state);
    }//if
}

void SharedLockFreeState::_commit_secondary(int mb_id, int64_t timestamp, LogTable& log_table, CommitMemory& commit_memory) {
    if (log_table[mb_id].empty()) {
        return;
    }//if

    // Find the first log whose timestamp is higher than the given timestamp
    auto it = std::upper_bound(log_table[mb_id].begin(), log_table[mb_id].end(), timestamp);

    if (it != log_table[mb_id].begin()) {
        // Commit involves storing the most updated log value into commit memory,
        // setting the timestamp time, and
        // erasing committed logs.
        std::advance(it, -1);
        _util.copy(commit_memory[mb_id].state, it->state);
        commit_memory[mb_id].timestamp = timestamp;
        std::advance(it, 1);

        log_table[mb_id].erase(log_table[mb_id].begin(), it);
    }//if
}

void SharedLockFreeState::_commit_primary(int64_t timestamp) {
#ifdef ENABLE_MULTI_THREADING
    std::lock_guard<std::mutex> log_guard(_primary_log_mutex);
#endif
//    DEBUG("The size of the log table of MB %d: %d", mb_id, _log_table[mb_id].size());

    if (_primary_log.empty()) {
        return;
    }//if

    // Find the first log whose timestamp is higher than the given timestamp
    auto it = std::upper_bound(_primary_log.begin(), _primary_log.end(), timestamp);

    if (it != _primary_log.begin()) {
        // Commit involves storing the most updated log value into commit memory,
        // setting the timestamp time, and
        // erasing committed logs.
        std::advance(it, -1);
        {
            // storing the most updated log value into commit memory
#ifdef ENABLE_MULTI_THREADING
            std::lock_guard<std::mutex> commit_guard(_primary_commit_mutex);
#endif
            _util.copy(_primary_commit.state, it->state);
            _primary_commit.timestamp = timestamp;
        }//{
        std::advance(it, 1);

        _primary_log.erase(_primary_log.begin(), it);
    }//if
}

void SharedLockFreeState::process_piggyback_message(Packet *p, LogTable& log_table, CommitMemory& commit_memory) {
    auto msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

    // Processing the primary state:
    _commit_primary(msg[_id]->timestamp);

    // Processing the secondary state set
    for (int i = 1; i <= _failure_count; ++i) {
        int mb_id = (_id - i + _chain_len) % _chain_len;

        _commit_secondary(mb_id, msg[mb_id]->last_commit, log_table, commit_memory);

        _log_secondary_state(msg[mb_id]->state, msg[mb_id]->timestamp, mb_id, log_table);
    }//for
}

void SharedLockFreeState::_capture_inoperation_state(Packet *p, int thread_id) {
    DEBUG("Capture _inoperation state");
#ifdef ENABLE_MULTI_THREADING
//    {// Wait until no thread is modifying inoperation state
//        std::unique_lock<std::mutex> lock(_modifying_phase_mtx);
//        _modifying_phase_cv.wait(lock, [&](){ return _modifying_phase == NOT_MODIFYING;});
//    }//{
//
//    {// Let other threads know that this thread with id thread_id
//        // is capturing the inoperation state
//        std::lock_guard<std::mutex> lock(_capture_inop_phase_mtx);
//        _capture_inop_phase = SET_K_TH_BIT(_capture_inop_phase, thread_id);
//    }//{

    // Capture the in-operation state
    PiggybackMessage *msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
    _util.copy(msg[_id]->state, _inoperation);
    msg[_id]->timestamp = CURRENT_TIMESTAMP;

    { // Capture in-operation state in the Log table
        std::lock_guard <std::mutex> log_guard(_primary_log_mutex);
        _primary_log.emplace_back(msg[_id]->timestamp, msg[_id]->state);
    }// {

//    {// Let other threads know that this thread with id thread_id
//        // has finished capturing the inoperation state
//        std::lock_guard<std::mutex> lock(_capture_inop_phase_mtx);
//        _capture_inop_phase = RESET_K_TH_BIT(_capture_inop_phase, thread_id);
//        _capture_inop_phase_cv.notify_all();
//    }//{
#else
    _log_table[_id].emplace_back(CURRENT_TIMESTAMP, _inoperation);
#endif
}

void SharedLockFreeState::_commit_timestamp(Packet *p) {
    PiggybackMessage *msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
#ifdef ENABLE_MULTI_THREADING
    std::lock_guard<std::mutex> commit_guard(_primary_commit_mutex);
#endif
    msg[_id]->last_commit = _primary_commit.timestamp;
}

void SharedLockFreeState::construct_piggyback_message(Packet *p, int thread_id) {
    DEBUG("Construct piggyback message");

    // The secondary state list and that of other middleboxes are already properly updated in
    // process_piggyback_message
    _capture_inoperation_state(p, thread_id);
    _commit_timestamp(p);
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

size_t SharedLockFreeState::log_table_length() {
    return this->_primary_log.size();
}

int SharedLockFreeState::read(int index) {
    return _inoperation[index];
}

void SharedLockFreeState::increment(int index) {
#ifdef ENABLE_MULTI_THREADING
//    {// Wait until no thread is capturing inoperation state
//        std::unique_lock<std::mutex> lock(_capture_inop_phase_mtx);
//        _capture_inop_phase_cv.wait(lock, [&](){ return _capture_inop_phase == NOT_CAPTURING; });
//    }//{
//
//    {// Let other threads know that this thread is modifying some state variable
//        std::lock_guard<std::mutex> lock(_modifying_phase_mtx);
//        _modifying_phase = SET_K_TH_BIT(_modifying_phase, index);
//    }// {

    ++_inoperation[index];

//    {// Let other threads know that this thread has finished processing state variable
//        std::lock_guard<std::mutex> lock(_modifying_phase_mtx);
//        _modifying_phase = RESET_K_TH_BIT(_modifying_phase, index);
//        _modifying_phase_cv.notify_all();
//    }// {
#else
    ++_inoperation[index];
#endif
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SharedLockFreeState)
