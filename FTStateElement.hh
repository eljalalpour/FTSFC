#pragma once

#include <click/config.h>
#include "FTTypes.hh"
#include <click/element.hh>

// The input port of a packet to be processed
#define INPUT_PORT_TO_PROCESS 0

// The input port from the middlebox. The packet is processed
#define INPUT_PORT_PROCESSED  1

#define OUTPUT_PORT_TO_MIDDLEBOX 0

#define OUTPUT_PORT_TO_NEXT_MIDDLEBOX 1

#define FT_STATE_ELEMENT_CLASS_NAME "FTStateElement"

#define CONF_ID            "ID"
#define CONF_FAILURE_COUNT "F"
#define CONF_VLAN_ID       "VLAN_ID"

CLICK_DECLS

class FTStateElement : public Element {
public:
    //TODO: must be configured
    FTMBId _id;// Must be initialized during the configuration phase
    VLANId _vlanId;// Must be initialized during the configuration phase
    int _failureCount;
    FTState _operationState; // Does not to be initialized
    FTModified _modified;  // Does not to be initialized
    FTPacketMBState _log;  // Does not to be initialized
    FTMBStates _committed; // Must be initialized in the configuration phase
    FTPacketMBPiggyBackedState _temp; // Does not to be initialized
    map<FTPacketId, Packet *> _packets; // Does not to be initialized

    /// Replicate the states of other middleBoxes and increment the ack value
    /// \param piggyBackedState
    void replicateStates(FTPacketMBPiggyBackedState &piggyBackedState);

    /// Transfer the states from log to the committed memory and release the log memory
    /// \param packetId The id of the packet
    /// \param MBId The id of the middlebox
    void commit(FTPacketId packetId, FTMBId MBId);

    /// Reset the modified map and temp memory for states
    inline void reset();

public:
    static const char *GET_CALL_BACK;
    static const char *PUT_CALL_BACK;
//    static const char *ROLLBACK_CALL_BACK;

    enum {
        GetCallBack,
        PutCallBack,
        RollbackCallBack
    };

    enum CALL_BACK_RESULT {
        FAILED = -1,
        SUCCESS = 0
    };

    //TODO: Remove the _failureCount and _id initialization,
    FTStateElement() : _failureCount(1), _id(2) {
        click_chatter("In FTStateElement Constructor!");
        FTState tmp1, tmp2;
        _committed[_id] = tmp1;
        _operationState = tmp2;
    }

    ~FTStateElement() {}

    const char *class_name() const { return "FTStateElement"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    //TODO: make sure that the connection type is push
    void push(int source, Packet *p);

    void rollback();

    void add_handlers();

    /// Get the value associated to @param key from the in-operation state or committed state by setting @param value.
    /// The function searches the in-operation state, and if the key is not found then the function searches
    /// the committed state. If the state is found the function returns true, otherwise the function returns false.
    /// \param key
    /// \param value
    /// \return true of @param key is found, otherwise false
    bool getPrimaryState(string key, string &value);

    /// Put @param value which is associated to @param key to the in-operation state
    /// \param key
    /// \param value
    /// \return true if @param key exists in the in-operation state, otherwise false
    bool putPrimaryState(string key, string value);

    /// Get the committed state of a middlebox with id @param mbId by setting @param state.
    /// \param mbId The id of middlebox
    /// \param state The state of middlebox to be set
    /// \return True if the state of middlebox with id @param mbId exists, otherwise false
    bool getCommittedState(FTMBId mbId, FTState &state);

    /// Put the state @param state to the committed state of middlebox @param mbId
    /// \param mbId The id of middlebox
    /// \param state The state of middlebox
    /// \return True if the state of middlebox with id @param mbId exists, otherwise false
    bool putCommittedState(FTMBId mbId, const FTState &state);

    /// Returns the committed state as a string
    /// \param e element object
    /// \param vparam
    /// \return The state
    static String getStateCallback(Element *e, void *user_data);

    static int putStateCallback(const String &data, Element *element, void *user_data, ErrorHandler *errh);
};

CLICK_ENDDECLS