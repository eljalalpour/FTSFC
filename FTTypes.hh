#pragma once

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/nvp.hpp>
#include <string>
#include <sstream>
#include <iostream>


using std::map;
using std::vector;
using std::string;
using std::stringstream;

#define PORTS_2_2 "2/2"
#define PORTS_2_1 "2/1"
#define PORTS_1_2 "1/2"

#define MB_ID_MAX 512

#define MAKE_UNIQUE_PACKET_ID(IP_OFFSET, IP_ID, SEQ_NUM) (((uint64_t)IP_OFFSET) << 48) | (((uint64_t)IP_ID) << 32) | ((uint64_t)SEQ_NUM)

// FTPacketId type identifies a packet uniquely
typedef uint64_t FTPacketId;

// FTMBId type identifies a MB uniquely
typedef uint16_t VLANId;

// FTMBId type identifies a MB uniquely
typedef uint8_t FTMBId;

// FTState type represents the state of a MB as a key value store
typedef map<string, string> FTState;

//shows if a variable has been modified
typedef map<string, bool> FTModified;

// FTMBStates type represents the state of a MB with id FTMBId
typedef map<FTMBId, FTState> FTMBStates;

// FTPacketMBState type represents the state changes that a packet has caused in a set of MBs
typedef map<FTPacketId, FTMBStates> FTPacketMBState;

class FTPiggyBackedState {
    friend class boost::serialization::access;

public:
    uint8_t ack;
    bool commit;
    FTState state;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int) {
        ar & commit & ack & BOOST_SERIALIZATION_NVP(state);
    }

    void serialize(FTPiggyBackedState &pbState, stringstream &ss) {
        boost::archive::binary_oarchive oa(ss);
        oa << pbState;
    }

    void deserialize(stringstream &ss, FTPiggyBackedState &pbState) {
        boost::archive::binary_iarchive ia(ss);
        ia >> pbState;
    }
};

// FTMBStates type represents the state of a MB with id FTMBId
typedef map<FTMBId, FTPiggyBackedState> FTMBPiggyBackedState;

// FTPacketMBState type represents the state changes that a packet has caused in a set of MBs
typedef map<FTPacketId, FTMBPiggyBackedState> FTPacketMBPiggyBackedState;
