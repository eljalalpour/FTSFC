#pragma once

#include "FTTypes.hh"
#include <click/config.h>
#include <click/element.hh>
#include <clicknet/tcp.h>
#include <boost/archive/archive_exception.hpp>

#define FROM_DUMP 0
#define FROM_TO_DEVICE 1
#define TO_FT_STATE 0
#define FT_APPENDER_CLASS_NAME "FTAppenderElement"

CLICK_DECLS

class FTAppenderElement : public Element {
private:
    FTPacketMBPiggyBackedState _temp;

public:
    FTAppenderElement();

    ~FTAppenderElement();

    const char *class_name() const { return "FTAppenderElement"; }

    //TODO: Change PORTS_1_1 to PORTS_2_1
    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    void push(int source, Packet *p);

    static void serializePiggyBacked(FTPacketMBPiggyBackedState &pbStates, stringstream &ss);

    static void deserializePiggyBacked(stringstream &ss, FTPacketMBPiggyBackedState &piggyBackedStates);

    static void deserializePiggyBacked(string& states, FTPacketMBPiggyBackedState &piggyBackedStates);

    static void serialize(FTMBStates &state, stringstream &ss);

    static void serialize(FTState &state, stringstream &ss);

    static void deserialize(stringstream &ss, FTMBStates &state);

    static void deserialize(stringstream &ss, FTState &state);

    static int  payloadOffset(Packet *p);

    /// Encode the states from the packet
    /// \param p The packet
    /// \param piggyBackedState The state that is being piggybacked
    /// \return an encoded packet
    static WritablePacket *encodeStates(Packet *p, FTPacketMBPiggyBackedState &piggyBackedState);

    /// Decode the states from the packet
    /// \param p The packet
    /// \param piggyBackedState The state that is being piggybacked
    /// \return the location of the payload
    static int decodeStates(Packet *p, FTPacketMBPiggyBackedState &piggyBackedState);

    /// Decode the states from the packet and return a packet without the state
    /// \param p The packet
    /// \param piggyBackedState The state that is being piggybacked
    /// \return The packet without the state
    static WritablePacket* decodeStatesRetPacket(Packet *p, FTPacketMBPiggyBackedState &piggyBackedState);

    /// Compress @arg data and write the compression result into @param buffer
    /// \param data To be compressed
    /// \param buffer Compressed
    static void compress(const std::string &data, std::string &buffer);

    /// Decompress @arg data and write the decompression result into @param buffer
    /// \param data To be decompressed
    /// \param buffer Decompressed
    static void decompress(const std::string &data, std::string &buffer);

    /// Get a unique identifier for a packet
    /// \param p The packet
    /// \return The unique identifier
    static FTPacketId getPacketId(Packet *p);

    // TODO: remove this function
    static void printState(FTState &state);
    static void printState(FTPiggyBackedState &state);
    static void printState(FTPacketMBPiggyBackedState &state);
};

CLICK_ENDDECLS