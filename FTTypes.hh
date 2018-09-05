#pragma once

#include "types.pb.hh"
#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <map>

using std::map;
using std::unordered_map;
using std::vector;
using std::string;
using std::stringstream;
using namespace FTSFC;

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

#define CURRENT_TIMESTAMP std::chrono::high_resolution_clock::now().time_since_epoch().count()

#define MAKE_UNIQUE_PACKET_ID(IP_OFFSET, IP_ID, SEQ_NUM) (((uint64_t)IP_OFFSET) << 48) | (((uint64_t)IP_ID) << 32) | ((uint64_t)SEQ_NUM)

// FTPacketId type identifies a packet uniquely
typedef uint64_t FTPacketId;

typedef int64_t FTTimestamp;

// FTMBId type identifies a MB uniquely
typedef uint16_t VLANId;

// FTMBId type identifies a MB uniquely
typedef int32_t FTMBId;

// FTState type represents the state of a MB as a key value store
typedef map<string, string> FTState;

// FTModified shows if a variable has been modified
typedef map<string, bool> FTModified;

class FTTimestampState {
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
public:
    int32_t ack;
    FTTimestamp last_commit;

    void set_last_commit() {
        this->last_commit = CURRENT_TIMESTAMP;
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

class RandomState {
public:
    void random_state(int count, FTState& state) {
        for (int i = 0; i < count; i++) {
            int k = random();
            int v = random();
            stringstream ss1, ss2;
            ss1 << k;
            ss2 << v;

            state[ss1.str()] = ss2.str();
        }//for
    }

    void random_ts_state(int count, FTTimestampState& ts_state) {
        ts_state.timestamp = CURRENT_TIMESTAMP;
        random_state(count, ts_state.state);
    }

    void random_piggy_back(int count, FTPiggyBackState& pb_state) {
        pb_state.last_commit = CURRENT_TIMESTAMP;

        FTTimestampState ts_state;
        random_ts_state(count, ts_state);
        pb_state.state = ts_state.state;
        pb_state.timestamp = ts_state.timestamp;
        pb_state.ack = 0;
    }

    void random_message(int mb_count, int state_count, FTPiggyBackMessage& msg) {
        for (int i = 0; i < mb_count; ++i) {
            random_piggy_back(state_count, msg[i]);
        }//for
    }
};

class Serializer {
public:
    string serialize(const FTState &ft_state) {
        State state;
        for (auto it = ft_state.begin(); it != ft_state.end(); ++it) {
            state.add_key(it->first);
            state.add_val(it->second);
        }//for

        return state.SerializeAsString();
    }

    void deserialize(FTState &ft_state, const string &str) {
        State state;
        state.ParseFromString(str);

        for (int i = 0; i < state.key_size(); ++i) {
            ft_state[state.key(i)] = state.val(i);
        }//for
    }

    string serialize(const FTTimestampState &ft_state) {
        TimestampState sr_tstate;

        sr_tstate.set_timestamp(ft_state.timestamp);

        for (auto it = ft_state.state.begin(); it != ft_state.state.end(); ++it) {
            sr_tstate.mutable_state()->add_key(it->first);
            sr_tstate.mutable_state()->add_val(it->second);
        }//for

        return sr_tstate.SerializeAsString();
    }

    void deserialize(FTTimestampState &ft_state, const string &str) {
        TimestampState sr_tstate;
        sr_tstate.ParseFromString(str);

        for (int i = 0; i < sr_tstate.state().key_size(); ++i) {
            ft_state.state[sr_tstate.state().key(i)] = sr_tstate.state().val(i);
        }//for
        ft_state.timestamp = sr_tstate.timestamp();
    }

    string serialize(const FTPiggyBackState &pb_state) {
        PiggyBackState sr_pb_state;

        sr_pb_state.set_ack(pb_state.ack);
        sr_pb_state.set_last_commit(pb_state.last_commit);
        sr_pb_state.set_timestamp(pb_state.timestamp);

        for (auto it = pb_state.state.begin(); it != pb_state.state.end(); ++it) {
            sr_pb_state.mutable_state()->add_key(it->first);
            sr_pb_state.mutable_state()->add_val(it->second);
        }//for

        return sr_pb_state.SerializeAsString();
    }

    void deserialize(FTPiggyBackState &ft_state, const string &str) {
        PiggyBackState sr_pb_state;
        sr_pb_state.ParseFromString(str);

        ft_state.ack = sr_pb_state.ack();
        ft_state.last_commit = sr_pb_state.last_commit();
        ft_state.timestamp = sr_pb_state.timestamp();

        for (int i = 0; i < sr_pb_state.state().key_size(); ++i) {
            ft_state.state[sr_pb_state.state().key(i)] = sr_pb_state.state().val(i);
        }//for
    }

    string serialize(const FTPiggyBackMessage &pb_msg) {
        PiggyBackMessage sr_pb_msg;

        for (auto it = pb_msg.begin(); it != pb_msg.end(); ++it) {
            sr_pb_msg.add_mb_id(it->first);

            PiggyBackState *pb_state = sr_pb_msg.add_pb_state();
            pb_state->set_ack(it->second.ack);
            pb_state->set_last_commit(it->second.last_commit);
            pb_state->set_timestamp(it->second.timestamp);

            for (auto it2 = it->second.state.begin(); it2 != it->second.state.end(); ++it2) {
                pb_state->mutable_state()->add_key(it2->first);
                pb_state->mutable_state()->add_val(it2->second);
            }//for
        }//for

        return sr_pb_msg.SerializeAsString();
    }

    void deserialize(FTPiggyBackMessage &ft_msg, const string &str) {
        PiggyBackMessage sr_pb_msg;
        sr_pb_msg.ParseFromString(str);

        for (int i = 0; i < sr_pb_msg.mb_id_size(); ++i) {
            ft_msg[sr_pb_msg.mb_id(i)].ack = sr_pb_msg.pb_state(i).ack();
            ft_msg[sr_pb_msg.mb_id(i)].last_commit = sr_pb_msg.pb_state(i).last_commit();
            ft_msg[sr_pb_msg.mb_id(i)].timestamp = sr_pb_msg.pb_state(i).timestamp();

            for (int j = 0; j < sr_pb_msg.pb_state(i).state().key_size(); ++j) {
                ft_msg[sr_pb_msg.mb_id(i)][sr_pb_msg.pb_state(i).state().key(j)] =
                        sr_pb_msg.pb_state(i).state().val(j);
            }//for
        }//for
    }
};
