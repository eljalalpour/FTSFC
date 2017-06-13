#include "FTStateElement.hh"
#include <click/args.hh>
#include <click/router.hh>
#include <stdio.h>
#include "FTAppenderElement.hh"

CLICK_DECLS

const char *FTStateElement::GET_CALL_BACK = "g";
const char *FTStateElement::PUT_CALL_BACK = "p";
//const char *FTStateElement::ROLLBACK_CALL_BACK = "r";

int FTStateElement::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh)
                .read(CONF_ID, _id)
                .read(CONF_VLAN_ID, _vlanId)
                .read(CONF_FAILURE_COUNT, _failureCount)
                .complete() < 0)
        return -1;

    //LOG("Configuration of state element: id: %d, vlanId: %d, f: %d\n", _id, _vlanId, _failureCount);

    return 0;
}

void FTStateElement::push(int source, Packet *p) {
    DEBUG("------------------------------\n");
    DEBUG("Begin FTStateElement %d:\n", _id);
    DEBUG("Receiving packet %llu from port %d\n", FTAppenderElement::getPacketId(p), source);

    if (source == INPUT_PORT_TO_PROCESS) {
        try {
            reset();
            WritablePacket *q = FTAppenderElement::decodeStatesRetPacket(p, _temp);
            replicate();

            //TODO: to remove begin
//            DEBUG("Temp size after replication: %d", _temp.size());
//            if (_temp.size() > 0) {
//                FTAppenderElement::printState(_temp);
//            }//if
            //TODO: to remove end

            p->kill();

            DEBUG("End FTStateElement %d\n", _id);
            DEBUG("------------------------------");
            output(OUTPUT_PORT_TO_MIDDLEBOX).push(q);
        }//try
        catch(...) {
            p->kill();
            DEBUG("Not A valid packet for our protocol\n");

            DEBUG("End FTStateElement %d\n", _id);
            DEBUG("------------------------------");
        }//catch
    }//if
    else if (source == INPUT_PORT_PROCESSED) {
        FTTimestampState primaryState;
        for (auto it = _modified.begin(); it != _modified.end(); ++it) {
            if (it->second) {
                primaryState[it->first] = _operationState[it->first];
            }//if
        }//for
        primaryState.timestamp = CURRENT_TIMESTAMP;
        FTPiggyBackState union_;
        auto primary_log = _log.find(_id);
        if (primary_log != _log.end() && primary_log->second.size() > 0) {
            union_.set_state(*primary_log->second.rbegin());
        }//if
        union_.time_union(primaryState);
        union_.ack = 1;
        auto committed_state_item = _committed.find(_id);

        union_.last_commit = (committed_state_item != _committed.end()) ?
                             committed_state_item->second.timestamp : 0;

        _temp[_id] = union_;
        add_to_log(_id, union_);

        // Write the changes state into log

        WritablePacket *q = FTAppenderElement::encodeStates(p, _temp);
        DEBUG("Packet size: %d\n", q->length());
        DEBUG("State going to the next middlebox (state size is %d):", _temp.size());

//        FTAppenderElement::printState(_temp);

        p->kill();

        DEBUG("End FTStateElement %d\n", _id);
        DEBUG("------------------------------");
        output(OUTPUT_PORT_TO_NEXT_MIDDLEBOX).push(q);
    }//else if
}

void FTStateElement::add_handlers() {
    for (int i = 0; i < this->_failureCount + 1; ++i) {
        stringstream ss;
        ss << GET_CALL_BACK << i;
        add_read_handler(String(ss.str().c_str()), getStateCallback, i);
    }//for
    add_write_handler(PUT_CALL_BACK, putStateCallback, PutCallBack, Handler::OP_WRITE);
}

void FTStateElement::add_to_log(FTMBId mb_id, FTTimestampState& state) {
    auto mb_item = _log.find(mb_id);
    if (mb_item == _log.end()) {
        FTTimestampStateList statelist;
        _log[mb_id] = statelist;
        mb_item = _log.find(mb_id);
    }//if

    mb_item->second.push_back(state);
}

void FTStateElement::replicate() {
    LOG("In state replication. temp size is %d!", _temp.size());

    for (auto mb_it = _temp.begin(); mb_it != _temp.end(); ++mb_it) {
        auto mb_id = mb_it->first;
        auto last_commit = (mb_id == _id) ? CURRENT_TIMESTAMP : mb_it->second.last_commit;
        auto ack = mb_it->second.ack;

        // If ack is less than f + 1, this means that piggybacked state is the secondary state
        if (ack < _failureCount + 1) {
            // Replicating the secondary state
            mb_it->second.ack++;
            add_to_log(mb_id, mb_it->second);

            // If the number of replication is f + 1, we clear the state because this replica is
            // the last replica of the replica group, and next replicas does not have to replicate this state.
            if (mb_it->second.ack == _failureCount + 1) {
                mb_it->second.state.clear();
            }//if
        }//if
        commit(mb_id, last_commit);
    }//for
}

void FTStateElement::commit(FTMBId MBId, FTTimestamp timestamp) {
    LOG("Committing the state of the middlebox '%d' for timestamp %llu", MBId, timestamp);

    auto log_mb_item = _log.find(MBId);
    if (log_mb_item == _log.end()) {
	FTTimestampStateList statelist;
        _log[MBId] = statelist;
	return;
    }//if

    // Find a log with the largest timestamp below the given timestamp
    int index = log_mb_item->second.size() - 1;
    LOG("Index before is %d", index);
    for (; index >= 0; index--) {
        if (timestamp >= log_mb_item->second[index].timestamp)
            break;
    }//for

    LOG("Index after is %d", index);

    // If a log is found, commit this log, and erase this log and all previous ones
    if (index >= 0) {
//        FTAppenderElement::printState(log_mb_item->second[index]);

        _committed[MBId].timestamp = timestamp;

        for (auto it = log_mb_item->second[index].begin(); it != log_mb_item->second[index].end(); ++it) {
            _committed[MBId][it->first] = it->second;
	    DEBUG("for MB %u commit %s Committed %s", MBId, it->first.c_str(), it->second.c_str());
        }//for

        DEBUG("Committing the state for timestamp: %llu", _committed[MBId].timestamp);
        DEBUG("Erasing %d log item(s) for middlebox %d\n", index + 1, MBId);

        log_mb_item->second.erase(log_mb_item->second.begin(), log_mb_item->second.begin() + index + 1);
    }//if
}

void FTStateElement::reset() {
    _temp.clear();
    for (auto it = _modified.begin(); it != _modified.end(); ++it) {
        it->second = false;
    }//for
}

void FTStateElement::rollback() {
    // Clear the logs
    this->_log.clear();

    // Rollback the in-operation state to committed state
    this->_operationState = _committed[this->_id].state;
}

bool FTStateElement::getPrimaryState(string key, string &value) {
//    //LOG("Get the state of key '%s'", key.c_str());

    bool found = false;
    auto item = _operationState.find(key);

    // First search the in-operation state
    if (item != _operationState.end()) {
        value = item->second;
        found = true;

//        //LOG("Found '%s' in the operation state. The value is: %s", key.c_str(), value.c_str());
    }//if
    else {
        // The key is not found in the operation state. Search the committed state now.
        auto found2 = _committed[_id].state.find(key);
        if (found2 != _committed[_id].state.end()) {
            value = found2->second;
            found = true;
//            //LOG("Found '%s' in the committed state. The value is: %s", key.c_str(), value.c_str());
        }//if
    }//else

//    if (!found)
        //LOG("Key '%s' is not found!\n", key.c_str());

    return found;
}

bool FTStateElement::putPrimaryState(string key, string value) {
    auto item = _operationState.find(key);
    _operationState[key] = value;
    _modified[key] = true;

    return (item != _operationState.end());
}

bool FTStateElement::getCommittedState(FTMBId mbId, FTTimestampState &state) {
    bool found = false;
    auto item = _committed.find(mbId);
    if (item != _committed.end()) {
        state = item->second;
        found = true;
    }//if
    return found;
}

bool FTStateElement::putCommittedState(FTMBId mbId, const FTTimestampState &ts_state) {
    bool found = false;
    auto item = _committed.find(mbId);
    if (item != _committed.end()) {
        item->second = ts_state;
    }//if
    else {
        _committed[mbId] = ts_state;
    }//else

    if (mbId == this->_id) {
        this->_operationState = ts_state.state;
    }//if

    return found;
}

/**
 * Get the state of a middlebox. The format of the result is as follows:
 * 0  1 ...
 * ID State
 * @param e The element whose handler is called
 * @param thunk The parameter that is specifies the middlebox
 * @return The state of the middlebox
 */
String FTStateElement::getStateCallback(Element *e, void *thunk) {
    //LOG("In get state callback!");
    FTStateElement *se = static_cast<FTStateElement *>(e);
    int param = intptr_t(thunk);

    std::cout << "Parameter is " << param << std::endl;

    //TODO: set the middleboxId with a valid id from the input
    FTMBId middleboxId = param;
    stringstream ss;
    string buffer;

    // Find the requested state
    FTTimestampState ts_state;
    if (se->getCommittedState(middleboxId, ts_state)) {

//        FTAppenderElement::printState(state);

        // Serialize and compress the state
        FTAppenderElement::serialize(ts_state, ss);
        FTAppenderElement::compress(ss.str(), buffer);
    }//if

    //TODO: Check the state of which replicas must be rollbacked, since the failed replica is in f + 1 replica-groups
    // We assume that get-state handler is called when a failure happens
    //so we need to rollback the states
    se->rollback();

    String str;
    str.append(middleboxId);
    str.append(buffer.c_str(), buffer.size());

    // Return the serialized and compressed state
    return str;
}

/**
 * Receiving and putting the state of a middlebox. The format of the result is as follows
 * @param data The state of the middlebox
 * @param e The element
 * @param user_data
 * @param errh
 * @return A zero or positive value for success, and a negative for the failure
 */
int FTStateElement::putStateCallback(const String &data, Element *e, void *user_data, ErrorHandler *errh) {
    //LOG("In rollback callback!");

    int result = SUCCESS;

    try {
        FTStateElement *se = static_cast<FTStateElement *>(e);

        FTMBId id = data[0];
        stringstream ss((char *)(data.c_str() + sizeof(unsigned char)), std::ios_base::binary);

        string buffer;
        FTAppenderElement::decompress(ss.str(), buffer);

        stringstream decSS(buffer, std::ios_base::binary);

        FTTimestampState state;
        FTAppenderElement::deserialize(decSS, state);
	
	DEBUG("put the state of middlebox %u!", id);
        FTAppenderElement::printState(state);

        se->putCommittedState(id, state);
    }//try
    catch(...){
        result = FAILED;
    }//catch

    return result;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(FTStateElement)
