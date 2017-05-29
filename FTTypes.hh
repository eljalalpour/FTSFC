#pragma once

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/nvp.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include <unordered_map>


using std::map;
using std::unordered_map;
using std::vector;
using std::string;
using std::stringstream;

//#define ENABLE_DEBUG 1
//#define ENABLE_LOG   1

#ifdef ENABLE_DEBUG
    #define DEBUG(...) click_chatter(__VA_ARGS__)
#else
    #define DEBUG(...)
#endif

#ifdef ENABLE_LOG
    #define LOG(...) click_chatter(__VA_ARGS__)
#else
    #define LOG(...)
#endif

#define PORTS_2_2 "2/2"
#define PORTS_2_1 "2/1"
#define PORTS_1_2 "1/2"

#define MB_ID_MAX 512

#define MAKE_UNIQUE_PACKET_ID(IP_OFFSET, IP_ID, SEQ_NUM) (((uint64_t)IP_OFFSET) << 48) | (((uint64_t)IP_ID) << 32) | ((uint64_t)SEQ_NUM)

// FTPacketId type identifies a packet uniquely
typedef uint64_t FTPacketId;

typedef uint64_t FTTimestamp;

// FTMBId type identifies a MB uniquely
typedef uint16_t VLANId;

// FTMBId type identifies a MB uniquely
typedef uint8_t FTMBId;

// FTState type represents the state of a MB as a key value store
typedef map<string, string> FTState;

// FTModified shows if a variable has been modified
typedef map<string, bool> FTModified;

// FTMBStates type represents the state of a MB with id FTMBId
typedef map<FTMBId, FTState> FTMBStates;

// FTPacketMBState type represents the state changes that a packet has caused in a set of MBs
typedef map<FTPacketId, FTMBStates> FTPacketMBState;

typedef map<string, FTTimestamp> FTKeyTimestamp;

typedef map<FTMBId, FTKeyTimestamp> FTMBKeyTimestamp;

class FTPiggyBackedState {
    friend class boost::serialization::access;

public:
    uint8_t ack;
    bool commit;
    FTTimestamp timestamp;
    FTState state;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int) {
        ar & commit & ack & timestamp & BOOST_SERIALIZATION_NVP(state);
    }

    void serialize(FTPiggyBackedState &pbState, stringstream &ss) {
        boost::archive::binary_oarchive oa(ss);
        oa << pbState;
    }

    void deserialize(stringstream &ss, FTPiggyBackedState &pbState) {
        boost::archive::binary_iarchive ia(ss);
        ia >> pbState;
    }

    void setTimeStamp() {
        this->timestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    }

    /// Perform minus operation and the state of @param first and the state of @param second (@param first=@param first-@param second)
    /// \param first
    /// \param second
    static void difference(FTPiggyBackedState& first, FTPiggyBackedState& second) {
        for (auto it = second.state.begin(); it != second.state.end(); ++it) {
            auto item = first.state.find(it->first);
            if (item != first.state.end())
                first.state.erase(item);
        }//for
    }

    /// Perform minus operation and the state of @param first and the state of @param second (@param result=@param first-@param second)
    /// \param first
    /// \param second
    /// \param result
    static void difference(FTPiggyBackedState& first, FTPiggyBackedState& second, FTPiggyBackedState& result) {
        result = first;
        difference(result, second);
    }
};

// FTMBStates type represents the state of a MB with id FTMBId
typedef map<FTMBId, FTPiggyBackedState> FTMBPiggyBackedState;

// FTPacketMBState type represents the state changes that a packet has caused in a set of MBs
typedef map<FTPacketId, FTMBPiggyBackedState> FTPacketMBPiggyBackedState;

//void difference(FTMBId mbId, FTMBPiggyBackedState& first, FTMBPiggyBackedState& second) {
//    FTPiggyBackedState::difference(first[mbId], second[mbId]);
//}
//
//void difference(FTMBPiggyBackedState& first, FTMBPiggyBackedState& second) {
//    for (auto it = second.begin(); it != second.end(); ++it) {
//        FTMBId mbId = it->first;
//        auto inFirst = first.find(mbId);
//        if (inFirst != first.end())
//            FTPiggyBackedState::difference(inFirst->second, second[mbId]);
//    }//for
//}
//
//void difference(FTMBId mbId, FTPacketMBPiggyBackedState& first, FTPacketMBPiggyBackedState& second) {
//    for (auto it = first.begin(); it != first.end(); ++it) {
//        for (auto it2 = second.begin(); it2 != second.end(); ++it2) {
//            difference(mbId, it->second, it2->second);
//        }//for
//    }//for
//}
//
//void difference(FTMBId mbId, FTPacketMBPiggyBackedState& first, FTPiggyBackedState& second) {
//    for (auto pkt_it = first.begin(); pkt_it != first.end(); ++pkt_it) {
//        auto mb_it = pkt_it->second.find(mbId);
//        if (mb_it == pkt_it->second.end())
//            continue;
//
//        FTPiggyBackedState::difference(mb_it->second, second);
//    }//for
//}
//
//void difference(FTMBId mbId, FTPacketMBPiggyBackedState& first, FTPiggyBackedState& second, FTPacketMBPiggyBackedState& result) {
//    result = first;
//    difference(mbId, result, second);
//}