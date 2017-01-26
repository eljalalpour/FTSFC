#include "FTStateElement.hh"
#include <click/args.hh>
#include <click/router.hh>
#include "FTAppenderElement.hh"

CLICK_DECLS

const char *FTStateElement::GET_CALL_BACK = "GET_CALL_BACK";

int FTStateElement::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh).read_or_set("ID", _id, rand() % MB_ID_MAX).complete() < 0)
        return -1;

    return 0;
}

void FTStateElement::push(int source, Packet *p) {
//    click_chatter("--------------------");
//    click_chatter("In FTStateElement:");
//    click_chatter("Receiving packet %llu from port %d", FTAppenderElement::getPacketId(p), source);

    if (source == INPUT_PORT_TO_PROCESS) {
        try {
            reset();
            FTPacketMBPiggyBackedState piggyBackedState;
            WritablePacket *q = FTAppenderElement::decodeStatesRetPacket(p,piggyBackedState);
            replicateStates(piggyBackedState);
            p->kill();
            output(OUTPUT_PORT_TO_MIDDLEBOX).push(q);
        }catch(...) {
            p->kill();
            click_chatter("In FTStateElement: Not A valid packet for our protocol");
        }
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
        PBState.commit = false;

        _log[packetId][_id] = primaryState;
        _temp[packetId][_id] = PBState;

        _packets[packetId] = p->uniqueify();

        WritablePacket *q = FTAppenderElement::encodeStates(p, _temp);

        p->kill();
        output(OUTPUT_PORT_TO_NEXT_MIDDLEBOX).push(q);
    }//else if
    click_chatter("--------------------");
}

void FTStateElement::add_handlers() {
    //TODO: Make sure that the flags for the handler should be Handler::CALM
    add_read_handler(GET_CALL_BACK, getStateCallback, GetCallBack, Handler::CALM);
}

void FTStateElement::replicateStates(FTPacketMBPiggyBackedState &piggyBackedState) {
//    click_chatter("In state replication");
//    click_chatter("this is the size of state: %d", piggyBackedState.size());
    for (auto it = piggyBackedState.begin(); it != piggyBackedState.end(); ++it) {
        auto packetId = it->first;

//        click_chatter("Replicating packet: %d", packetId);

        for (auto it2 = it->second.begin(); it2 != it->second.end(); /*no increment*/) {
            auto MBId = it2->first;
            bool itemErased = false;

            // Check if the state is of the current MB
            if (MBId == _id) {
                //committing the primary states here
                if (it2->second.commit) {
                    //The middlebox visits its state in the third phase of protocol
                    it->second.erase(it2++);
                    //TODO check if we should release the memory of it(iterator)
                    itemErased = true;
                    // erase the log whenever the state of the packet is committed for every middlebox
                    _log.erase(packetId);

                }//if

                else if (it2->second.ack == _failureCount + 1) {
                    //The middlebox visits its state in the second phase of protocol
                    commit(packetId, MBId);
                    it2->second.commit = true;
                    _temp[packetId][MBId] = it2->second;
                }//else if
            }//if
            else {
                //replicating the secondary states here
//                click_chatter("Replicating secondary state!");

                if (it2->second.ack != _failureCount + 1) {
                    _log[packetId][MBId] = it2->second.state;

                    FTAppenderElement::printState(it2->second.state);

                    ++(it2->second.ack);
                    if (it2->second.ack == _failureCount + 1) {
                        it2->second.state.clear();
                    }//if
                }//if
                _temp[packetId][MBId] = it2->second;
            }//else

            // Increment iterator here
            if (!itemErased) {
                ++it2;
            }//if
        }//for
    }//for
}

void FTStateElement::commit(FTPacketId packetId, FTMBId MBId) {
//    click_chatter("Committing the state of the middlebox '%d' for the packet id '%d", MBId, packetId);
    for (auto it = _log[packetId][MBId].begin(); it != _log[packetId][MBId].end(); ++it) {
        _committed[MBId][it->first] = it->second;
//        click_chatter("'%s':'%s", it->first.c_str(), it->second.c_str());
    }//for
//    _log[packetId].erase(MBId);
//    if (_log[packetId].size() == 0) {
//        _log.erase(packetId);
//    }//if
}

void FTStateElement::reset() {
    _temp.clear();
    for (auto it = _modified.begin(); it != _modified.end(); ++it) {
        it->second = false;
    }//for
}

bool FTStateElement::getPrimaryState(string key, string &value) {
//    click_chatter("Get the state of key '%s'", key.c_str());

    bool found = false;
    auto item = _operationState.find(key);

    // First search the in-operation state
    if (item != _operationState.end()) {
        value = item->second;
        found = true;

//        click_chatter("Found '%s' in the operation state. The value is: %s", key.c_str(), value.c_str());
    }//if
    else {
        // The key is not found in the operation state. Search the committed state now.
        auto found2 = _committed[_id].find(key);
        if (found2 != _committed[_id].end()) {
            value = found2->second;
            found = true;
//            click_chatter("Found '%s' in the committed state. The value is: %s", key.c_str(), value.c_str());
        }//if
    }//else

//    if (!found)
//        click_chatter("Key '%s' is not found!", key.c_str());

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

    return found;
}

String FTStateElement::getStateCallback(Element *e, void *vparam) {
    FTStateElement *se = static_cast<FTStateElement *>(e);

    if ((intptr_t) vparam != GetCallBack)
        return "";

    stringstream ss;
    FTAppenderElement::serialize(se->_committed, ss);

    String res(ss.str().c_str());
    return res;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTStateElement)