#ifndef FTSFC_DEFS_HH
#define FTSFC_DEFS_HH

/// Assumptions:
///     - The state of a middlebox is a FIXED int array with 8 values (32 bytes)
///     - The piggyback message always contains the piggback state of 4 middleboxes


#define STATE_LEN 8
#define MB_LEN    4

#include <vector>
#include <unordered_map>
#include <chrono>

#define ENABLE_DEBUG 1

#ifdef ENABLE_DEBUG
#define DEBUG(...) click_chatter(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

#define CURRENT_TIMESTAMP std::chrono::high_resolution_clock::now().time_since_epoch().count()

typedef int State[STATE_LEN];

typedef struct {
    int64_t timestamp;
    State state;
} TimestampState;

typedef struct {
    short ack;
    int64_t last_commit;
    int64_t timestamp;
    State state;
} PiggyBackState;

typedef PiggyBackState PiggyBackMessage[MB_LEN];

typedef std::vector<TimestampState> TimestampStateList;

typedef TimestampStateList Log[MB_LEN];

typedef TimestampState Committed[MB_LEN];

#define CAST_TO_BYTES(x)              reinterpret_cast<unsigned char *>(&x)
#define CAST_TO_STATE(x)              reinterpret_cast<State*>(x)
#define CAST_TO_TIMESTAMP_STATE(x)    reinterpret_cast<TimestampState*>(x)
#define CAST_TO_PIGGY_BACK_STATE(x)   reinterpret_cast<PiggyBackState*>(x)
#define CAST_TO_PIGGY_BACK_MESSAGE(x) reinterpret_cast<PiggyBackMessage*>(x)

class Util {
public:
    inline void init(State &s) {
        memset(&s, 0, sizeof(State));
    }

    inline void init(TimestampState &s) {
        memset(&s, 0, sizeof(TimestampState));
    }

    inline void init(PiggyBackState &s) {
        memset(&s, 0, sizeof(PiggyBackState));
    }

    inline void init(PiggyBackMessage &s) {
        memset(&s, 0, sizeof(PiggyBackMessage));
    }

    inline void copy(State& y, State& x) {
        memcpy(&y, &x, sizeof(State));
    }

    inline void copy(TimestampState& y, TimestampState& x) {
        memcpy(&y, &x, sizeof(TimestampState));
    }

    inline void copy(PiggyBackState& y, PiggyBackState& x) {
        memcpy(&y, &x, sizeof(PiggyBackState));
    }

    inline void copy(PiggyBackMessage& y, PiggyBackMessage& x) {
        memcpy(&y, &x, sizeof(PiggyBackState));
    }
};

class Serializer {
public:
    inline void serialize(const State &s, unsigned char *ser) {
        memcpy(ser, &s, sizeof(State));
    }

    inline void deserialize(State &s, const unsigned char* ser) {
        memcpy(&s, ser, sizeof(State));
    }

    inline void serialize(const TimestampState &s, unsigned char *ser) {
        memcpy(ser, &s, sizeof(TimestampState));
    }

    inline void deserialize(TimestampState &s, const unsigned char* ser) {
        memcpy(&s, ser, sizeof(TimestampState));
    }

    inline void serialize(const PiggyBackState &s, unsigned char *ser) {
        memcpy(ser, &s, sizeof(PiggyBackState));
    }

    inline void deserialize(PiggyBackState &s, const unsigned char* ser) {
        memcpy(&s, ser, sizeof(PiggyBackState));
    }

    inline void serialize(const PiggyBackMessage &s, unsigned char *ser) {
        memcpy(ser, &s, sizeof(PiggyBackState));
    }

    inline void deserialize(PiggyBackMessage &s, const unsigned char* ser) {
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
        Util util;
        pb_state.last_commit = CURRENT_TIMESTAMP;
        TimestampState ts_state;
        random_ts_state(ts_state);
        util.copy(pb_state.state, ts_state.state);
        pb_state.timestamp = ts_state.timestamp;
        pb_state.ack = 0;
    }

    void random_message(PiggyBackMessage& msg) {
        for (int i = 0; i < MB_LEN; ++i) {
            random_piggy_back(msg[i]);
        }//for
    }
};

#endif //FTSFC_DEFS_HH
