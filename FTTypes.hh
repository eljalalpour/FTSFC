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

#define ENABLE_DEBUG 1
#define ENABLE_LOG   1

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

#define CURRENT_TIMESTAMP std::chrono::high_resolution_clock::now().time_since_epoch().count()

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

class FTTimestampState {
    friend class boost::serialization::access;
public:
    FTState state;
    FTTimestamp timestamp;

    FTTimestampState() : timestamp(0) { }

    void set_timestamp() {
        this->timestamp = CURRENT_TIMESTAMP;
    }

    string& operator[] (const string& index) {
        return state[index];
    }

    template<class Archive>
    void serialize(Archive &ar, const unsigned int) {
        ar & timestamp & BOOST_SERIALIZATION_NVP(state);
    }

    void serialize(FTTimestampState& ts_state, stringstream& ss) {
        boost::archive::binary_oarchive oa(ss);
        oa << ts_state;
    }

    void deserialize(stringstream& ss, FTTimestampState& ts_state) {
        boost::archive::binary_iarchive ia(ss);
        ia >> ts_state;
    }

    void time_union(FTTimestampState& st_state) {
        if (st_state.timestamp < timestamp)
            return;

        for (auto it = st_state.begin(); it != st_state.end(); ++it) {
            (*this)[it->first] = it->second;
        }//for
        timestamp = st_state.timestamp;
    }

    FTState::iterator begin() {
        return state.begin();
    }

    FTState::iterator end() {
        return state.end();
    }
};

class FTPiggyBackState: public FTTimestampState {
    friend class boost::serialization::access;
public:
    uint8_t ack;
    FTTimestamp last_commit;

    void set_last_commit() {
        this->last_commit = CURRENT_TIMESTAMP;
    }

    template<class Archive>
    void serialize(Archive &ar, const unsigned int) {
        ar & last_commit & ack & timestamp & BOOST_SERIALIZATION_NVP(state);
    }

    void serialize(FTPiggyBackState& pb_state, stringstream& ss) {
        boost::archive::binary_oarchive oa(ss);
        oa << pb_state;
    }

    void deserialize(stringstream& ss, FTPiggyBackState& pb_state) {
        boost::archive::binary_iarchive ia(ss);
        ia >> pb_state;
    }

    void set_state(FTTimestampState& ts_state) {
        state = ts_state.state;
        timestamp = ts_state.timestamp;
    }
};

typedef vector<FTTimestampState> FTTimestampStateList;

typedef map<FTMBId, FTTimestampStateList> FTLog;

typedef map<FTMBId, FTTimestampState> FTCommitted;

typedef map<FTMBId, FTPiggyBackState> FTPiggyBackMessage;

//class FTCommitted {
//    friend class boost::serialization::access;
//private:
//    map<FTMBId, FTTimestampState> ts_states;
//
//public:
//    template<class Archive>
//    void serialize(Archive &ar, const unsigned int) {
//        ar & BOOST_SERIALIZATION_NVP(ts_states);
//    }
//
//    void serialize(FTCommitted& committed, stringstream& ss) {
//        boost::archive::binary_oarchive oa(ss);
//        oa << committed;
//    }
//
//    void deserialize(stringstream& ss, FTCommitted& committed) {
//        boost::archive::binary_iarchive ia(ss);
//        ia >> committed;
//    }
//
//    FTTimestampState& operator[] (const FTMBId& index) {
//        return ts_states[index];
//    }
//
//    map<FTMBId, FTTimestampState>::iterator find(const FTMBId& index) {
//        return ts_states.find(index);
//    };
//
//    map<FTMBId, FTTimestampState>::iterator begin() {
//        return ts_states.begin();
//    };
//
//    map<FTMBId, FTTimestampState>::iterator end() {
//        return ts_states.end();
//    };
//};
//
//class FTPiggyBackMessage {
//    friend class boost::serialization::access;
//
//public:
//    map<FTMBId, FTPiggyBackState> _pb_states;
//
//public:
//    void update(const map<FTMBId, FTPiggyBackState>::iterator& begin,
//                const map<FTMBId, FTPiggyBackState>::iterator& end) {
//        for (auto it = begin; it != end; it++)
//            _pb_states[it->first] = it->second;
//    }
//
//    FTTimestamp last_mb_commit_timestamp() {
//        return (_pb_states.size() > 0) ?
//               _pb_states.rbegin()->second.last_commit : 0;
//    }
//
//    FTTimestamp last_mb_timestamp() {
//        FTTimestamp result = 0;
//        if (_pb_states.size() > 0)
//            result = _pb_states.rbegin()->second.timestamp;
//
//        return result;
//    }
//
//    template<class Archive>
//    void serialize(Archive &ar, const unsigned int) {
//        ar & BOOST_SERIALIZATION_NVP(_pb_states);
//    }
//
//    void serialize(FTPiggyBackMessage& msg, stringstream& ss) {
//        boost::archive::binary_oarchive oa(ss);
//        oa << msg;
//    }
//
//    void deserialize(stringstream& ss, FTPiggyBackMessage& msg) {
//        boost::archive::binary_iarchive ia(ss);
//        ia >> msg;
//    }
//
//    FTPiggyBackState& operator[] (const FTMBId& index) {
//        return _pb_states[index];
//    }
//
//    map<FTMBId, FTPiggyBackState>::iterator find(const FTMBId& index) {
//        return _pb_states.find(index);
//    };
//
//    map<FTMBId, FTPiggyBackState>::iterator begin() {
//        return _pb_states.begin();
//    };
//
//    map<FTMBId, FTPiggyBackState>::iterator end() {
//        return _pb_states.end();
//    };
//
//    map<FTMBId, FTPiggyBackState>::reverse_iterator rbegin() {
//        return _pb_states.rbegin();
//    };
//
//    map<FTMBId, FTPiggyBackState>::reverse_iterator rend() {
//        return _pb_states.rend();
//    };
//
//    void clear() {
//        _pb_states.clear();
//    }
//
//    size_t size() {
//        _pb_states.size();
//    }
//};