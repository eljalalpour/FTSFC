#pragma once

#include "FTTypes.hh"
#include <click/config.h>
#include <click/element.hh>
#include <clicknet/tcp.h>
#include <mutex>

#define FROM_DUMP 0
#define FROM_TO_DEVICE 1
#define TO_FT_STATE 0
#define FT_APPENDER_CLASS_NAME "FTAppenderElement"

#define IP_PACKET_OFFSET 14
#define IP_PACKET_AFTER_VLAN_OFFSET 18

CLICK_DECLS

class FTAppenderElement : public Element {
private:
    FTPiggyBackMessage _temp;
    std::mutex _lock;
public:
    FTAppenderElement();

    ~FTAppenderElement();

    const char *class_name() const { return "FTAppenderElement"; }

    const char *port_count() const { return PORTS_2_1; }

    const char *processing() const { return AGNOSTIC; }

    void push(int source, Packet *p);

    void append(FTPiggyBackMessage& state);

    static void serializePiggyBacked(FTPiggyBackMessage &pbStates, string &ss);

    static void deserializePiggyBacked(string& states, FTPiggyBackMessage &piggyBackedStates);

    static void serialize(FTPiggyBackState &state, string &ss);

    static void serialize(FTTimestampState &state, string &ss);

    static void deserialize(string &ss, FTPiggyBackState &state);

    static void deserialize(string &ss, FTTimestampState &state);

    static int  payloadOffset(Packet *p);

    /// Encode the states from the packet
    /// \param p The packet
    /// \param piggyBackedState The state that is being piggybacked
    /// \return an encoded packet
    static WritablePacket *encodeStates(Packet *p, FTPiggyBackMessage& msg);

    /// Decode the states from the packet
    /// \param p The packet
    /// \param piggyBackedState The state that is being piggybacked
    /// \return the location of the payload
    static int decodeStates(Packet *p, FTPiggyBackMessage &piggyBackedState);

    /// Decode the states from the packet and return a packet without the state
    /// \param p The packet
    /// \param piggyBackedState The state that is being piggybacked
    /// \return The packet without the state
    static WritablePacket* decodeStatesRetPacket(Packet *p, FTPiggyBackMessage &piggyBackedState);

    /// Serialize and compress state
    /// \param state The state to be encoded
    /// \param buffer The encoded byte stream
    static void encode(FTTimestampState& state, string& buffer);

    /// Deserialize and decompress state
    /// \param data The compressed serialized state
    /// \param state The state
    static void decode(const string& data, FTTimestampState& state);

    static void decode(const char* data, int size, FTTimestampState& state);

    /// Get a unique identifier for a packet
    /// \param p The packet
    /// \return The unique identifier
    static FTPacketId getPacketId(Packet *p, int ip_offset = IP_PACKET_OFFSET);

    // TODO: remove this function
    static void printState(FTState &state);
    static void printState(FTTimestampState &state);
    static void printState(FTPiggyBackState &state);
    static void printState(FTPiggyBackMessage &state);
};

CLICK_ENDDECLS
