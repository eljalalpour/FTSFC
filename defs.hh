#ifndef FTSFC_DEFS_HH
#define FTSFC_DEFS_HH

/// Assumptions:
///     - The state of a middlebox is a FIXED int array with 8 values (32 bytes)
///     - The piggyback message always contains the piggback state of 4 middleboxes


#define STATE_VARS_LEN  8
#define MIDDLEBOXES_LEN 4

#include <vector>
#include <unordered_map>
#include <chrono>

#define Timestamp int64_t
#define CURRENT_TIMESTAMP std::chrono::high_resolution_clock::now().time_since_epoch().count()

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


typedef int[STATE_VARS_LEN] State;

typedef struct {
    Timestamp timestamp;
    State state;
} TimestampState;

typedef struct {
    short ack;
    Timestamp last_commit;
    Timestamp timestamp;
    State state;
} PiggyBackState;

typedef PiggyBackState[MBS_LEN] PiggyBackMessage;

typedef std::vector<TimestampState> TimestampStateList;

typedef FTTimestampStateList[MIDDLEBOXES_LEN] Log;

typedef TimestampState[MIDDLEBOXES_LEN] Committed;


#define CAST_TO_BYTES(x)              reinterpret_cast<unsigned char *>(&x)
#define CAST_TO_STATE(x)              reinterpret_cast<State*>(x)
#define CAST_TO_TIMESTAMP_STATE(x)    reinterpret_cast<TimestampState*>(x)
#define CAST_TO_PIGGY_BACK_STATE(x)   reinterpret_cast<PiggyBackState*>(x)
#define CAST_TO_PIGGY_BACK_MESSAGE(x) reinterpret_cast<PiggyBackMessage*>(x)

class Initializer {
public:
    void init(State &s) {
        memset(&s, 0, sizeof(State));
    }

    void init(TimestampState &s) {
        memset(&s, 0, sizeof(TimestampState));
    }

    void init(PiggyBackState &s) {
        memset(&s, 0, sizeof(PiggyBackState));
    }

    void init(PiggyBackMessage &s) {
        memset(&s, 0, sizeof(PiggyBackMessage));
    }
};

class Serializer {
public:
    void serialize(const State &s, unsigned char *ser) {
        memcpy(ser, &s, sizeof(State));
    }

    void deserialize(State &s, const unsigned char* ser) {
        memcpy(&s, ser, sizeof(State));
    }

    void serialize(const TimestampState &s, unsigned char *ser) {
        memcpy(ser, &s, sizeof(TimestampState));
    }

    void deserialize(TimestampState &s, const unsigned char* ser) {
        memcpy(&s, ser, sizeof(TimestampState));
    }

    void serialize(const PiggyBackState &s, unsigned char *ser) {
        memcpy(ser, &s, sizeof(PiggyBackState));
    }

    void deserialize(PiggyBackState &s, const unsigned char* ser) {
        memcpy(&s, ser, sizeof(PiggyBackState));
    }

    void serialize(const PiggyBackMessage &s, unsigned char *ser) {
        memcpy(ser, &s, sizeof(PiggyBackState));
    }

    void deserialize(PiggyBackMessage &s, const unsigned char* ser) {
        memcpy(&s, ser, sizeof(PiggyBackMessage));
    }
};

class RandomState {
public:
    void random_state(State& s) {
        for (int i = 0; i < STATE_LEN; i++) {
            s[i] = random();
        }//for
    }

    void random_ts_state(TimestampState& ts_state) {
        ts_state.timestamp = CURRENT_TIMESTAMP;
        random_state(ts_state.state);
    }

    void random_piggy_back(PiggyBackState& pb_state) {
        pb_state.last_commit = CURRENT_TIMESTAMP;

        TimestampState ts_state;
        random_ts_state(ts_state);
        pb_state.state = ts_state.state;
        pb_state.timestamp = ts_state.timestamp;
        pb_state.ack = 0;
    }

    void random_message(PiggyBackMessage& msg) {
        for (int i = 0; i < MIDDLEBOXES_LEN; ++i) {
            random_piggy_back(msg[i]);
        }//for
    }
};


#endif //FTSFC_DEFS_HH
