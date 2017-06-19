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
    int _failureCount;
    FTState _operationState; // Does not to be initialized
    FTModified _modified;  // Does not to be initialized
    FTLog _log;  // Does not to be initialized
    FTCommitted _committed; // Must be initialized in the configuration phase
    FTPiggyBackMessage _temp; // Does not to be initialized

    /// Replicate the states of other middleboxes, increment their ack value, and remove the FTState if necessary
    void replicate();

    /// Commit the states from log to the committed memory if the log entry has a
    /// timestamp less than @param commit_timestamp. This function also releases the log memory
    /// \param MBId The middlebox id
    /// \param commit_timestamp The commit timestamp
    void commit(FTMBId MBId, FTTimestamp commit_timestamp);

    /// Reset the modified map and temp memory for states
    inline void reset();

    void add_to_log(FTMBId mb_id, FTTimestampState& state);

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

    FTStateElement() {
        click_chatter("In FTStateElement Constructor!");
        _committed[_id] = FTTimestampState();
        _operationState = FTState();
    }

    ~FTStateElement() {}

    const char *class_name() const { return "FTStateElement"; }

    const char *port_count() const { return PORTS_2_2; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    //TODO: make sure that the connection type is push
    void push(int source, Packet *p);

    void rollback();

//    void add_handlers();

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
    bool getCommittedState(FTMBId mbId, FTTimestampState &state);

    /// Put the state @param state to the committed state of middlebox @param mbId
    /// \param mbId The id of middlebox
    /// \param state The state of middlebox
    /// \return True if the state of middlebox with id @param mbId exists, otherwise false
    bool putCommittedState(FTMBId mbId, const FTTimestampState &state);

//    /// Returns the committed state as a string
//    /// \param e element object
//    /// \param vparam
//    /// \return The state
//    static String getStateCallback(Element *e, void *user_data);
//
//    static int putStateCallback(const String &data, Element *element, void *user_data, ErrorHandler *errh);
};

CLICK_ENDDECLS
