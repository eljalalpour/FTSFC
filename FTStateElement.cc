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

            DEBUG("State received from FTAppender or previous replica\n");
            FTAppenderElement::printState(_temp);

            replicateStates();
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
        FTState primaryState;
        for (auto it = _modified.begin(); it != _modified.end(); ++it) {
            if (it->second) {
                primaryState[it->first] = _operationState[it->first];
            }//if
        }//for
        auto packetId = FTAppenderElement::getPacketId(p);
        FTPiggyBackedState PBState;
        PBState.state = primaryState;
        PBState.ack = 1;
        PBState.commit = (PBState.ack > this->_failureCount);
        PBState.setTimeStamp();

        if (!PBState.commit) {
            _log[packetId][_id] = PBState;
        }//if

        for (auto it = _log.begin(); it != _log.end(); ++it) {
            for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                if (it2->second.ack < _failureCount + 1) {
                    _temp[it->first][it2->first] = it2->second;
                }//if
                else if (it2->second.ack == _failureCount + 1) {
                    FTPiggyBackedState sState = it2->second;
                    sState.state.clear();
                    _temp[it->first][it2->first] = sState;
                }//else if
            }//for
        }//for
        DEBUG("State going to the next middlebox (state size is %d):", _temp.size());
        FTAppenderElement::printState(_temp);

        _packets[packetId] = p->uniqueify();
        WritablePacket *q = FTAppenderElement::encodeStates(p, _temp);
        DEBUG("Packet size: %d\n", q->length());

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

void FTStateElement::replicateStates() {
    //LOG("In state replication\n");

    FTMBKeyTimestamp _committed_time;

//    LOG("this is the size of state: %d\n", _temp.size());
    for (auto it = _temp.begin(); it != _temp.end(); /* no increment */) {
        auto packetId = it->first;

//        LOG("Replicating packet: %llu\n", packetId);

        for (auto it2 = it->second.begin(); it2 != it->second.end(); /*no increment*/) {
            auto MBId = it2->first;
            bool itemErased = false;

            // Check if the state is of the current MB
            if (MBId == _id) {
                //committing the primary states here
                if (it2->second.commit) {
                    //The middlebox visits its state in the third phase of protocol
                    LOG("Removing the information of packet %llu and middlebox %d\n", it->first, it2->first);
                    it->second.erase(it2++);
                    //TODO: check if we should release the memory of it(iterator)
                    itemErased = true;
                    // erase the log whenever the state of the packet is committed for every middlebox
                    _log.erase(packetId);
                }//if

                else if (it2->second.ack == _failureCount + 1) {
                    //The middlebox visits its state in the second phase of protocol
                    LOG("Committing the state of packet %llu and middlebox %d", it->first, it2->first);
                    commit(packetId, MBId, _committed_time);
                    it2->second.commit = true;

                    //TODO: new-begin
                    //TODO: Make the following fix a bit cleaner
                    // We should not remove the primary state from the log till the third phase,
                    //to fix that we do as follows
                    _log[packetId][MBId]= it2->second;
                    it->second.erase(it2++);
                    itemErased = true;
                    //TODO: new-end

                    LOG("Printing temp for debug:\n");
//                    FTAppenderElement::printState(_temp);
                }//else if
            }//if
            else {
                //replicating the secondary states here
                if (it2->second.ack != _failureCount + 1) {
                    LOG("Replicating secondary state (ack is %d)!\n", (int)it2->second.ack);

//                    FTAppenderElement::printState(it2->second.state);
                    ++(it2->second.ack);

                    _log[packetId][MBId] = it2->second;

                    LOG("Ack of packet %llu, and middlebox %d is %d\n", packetId, MBId, _log[packetId][MBId].ack);

                    it->second.erase(it2++);
                    itemErased = true;

//                    if (it2->second.ack == _failureCount + 1) {
//                        it2->second.state.clear();
//                    }//if
                }//if
                else if (it2->second.commit) {
                    //Commiting secondary state
                    commit(packetId, MBId, _committed_time);
                }//else
//                _temp[packetId][MBId] = it2->second;
            }//else

            // Increment iterator here
            if (!itemErased) {
                ++it2;
            }//if
        }//for

        if (it->second.size() == 0) {
            _temp.erase(it++);
        }//if
        else {
            ++it;
        }//else
    }//for
}

void FTStateElement::commit(FTPacketId packetId, FTMBId MBId, FTMBKeyTimestamp& committed_time) {
    //LOG("Committing the state of the middlebox '%d' for the packet id %llu", MBId, packetId);
    FTTimestamp time = _log[packetId][MBId].timestamp;

    auto middlebox_ct = committed_time.find(MBId);
    if (middlebox_ct == committed_time.end()) {
        FTKeyTimestamp empty;
        committed_time[MBId] = empty;
        middlebox_ct = committed_time.find(MBId);
    }//if

    for (auto log_it = _log[packetId][MBId].state.begin(); log_it != _log[packetId][MBId].state.end(); ++log_it) {
        auto ct = middlebox_ct->second.find(log_it->first);
        if (ct == middlebox_ct->second.end() || time > ct->second) {
            _committed[MBId][log_it->first] = log_it->second;
            middlebox_ct->second[log_it->first] = time;
            //LOG("'%s':'%s", log_it->first.c_str(), log_it->second.c_str());
        }//if
//        else if (time > ct->second){
//            _committed[MBId][log_it->first] = log_it->second;
//            middlebox_ct->second[log_it->first] = time;
//            //LOG("'%s':'%s", log_it->first.c_str(), log_it->second.c_str());
//        }//else if
    }//for
    _log[packetId].erase(MBId);
    if (_log[packetId].size() == 0) {
        _log.erase(packetId);
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
    this->_operationState = _committed[this->_id];
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
        auto found2 = _committed[_id].find(key);
        if (found2 != _committed[_id].end()) {
            value = found2->second;
            found = true;
//            //LOG("Found '%s' in the committed state. The value is: %s", key.c_str(), value.c_str());
        }//if
    }//else

    if (!found)
        //LOG("Key '%s' is not found!\n", key.c_str());

    return found;
}

bool FTStateElement::putPrimaryState(string key, string value) {
    auto item = _operationState.find(key);
    _operationState[key] = value;
    _modified[key] = true;

    return (item != _operationState.end());
}

bool FTStateElement::getCommittedState(FTMBId mbId, FTState &state) {
    bool found = false;
    auto item = _committed.find(mbId);
    if (item != _committed.end()) {
        state = item->second;
        found = true;
    }//if
    return found;

//    state["khar"] = "gav";
//    state["sag"] = "gurba";
//    return true;
}

bool FTStateElement::putCommittedState(FTMBId mbId, const FTState &state) {
    bool found = false;
    auto item = _committed.find(mbId);
    if (item != _committed.end()) {
        item->second = state;
    }//if
    else {
        _committed[mbId] = state;
    }//else

    if (mbId == this->_id) {
        this->_operationState = state;
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
    FTState state;
    if (se->getCommittedState(middleboxId, state)) {

//        FTAppenderElement::printState(state);

        // Serialize and compress the state
        FTAppenderElement::serialize(state, ss);
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

        FTState state;
        FTAppenderElement::deserialize(decSS, state);

//        FTAppenderElement::printState(state);

        se->putCommittedState(id, state);
    }//try
    catch(...){
        result = FAILED;
    }//catch

    return result;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTStateElement)