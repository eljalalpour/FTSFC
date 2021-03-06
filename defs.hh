#ifndef FTSFC_DEFS_HH
#define FTSFC_DEFS_HH

/// Assumptions:
///     - The state of a middlebox is a FIXED int16 array with 8 values (16 bytes)
///     - The piggyback message always contains the piggback state of 5 middleboxes
///     - Middleboxes must not modify packets

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <mutex>
#include <vector>
#include <click/packet.hh>
#include <click/glue.hh>
#include "elided_spin_lock.hh"

/// Useful  definitions
#define PORTS_2_1 "2/1"
#define PORTS_1_2 "1/2"
#define PORTS_2_2 "2/2"

// Note that n and k must be of type size_t
#define K_TH_BIT(n,k)       ((n & ( 1 << k )) >> k)
#define RESET_K_TH_BIT(n,k) (n &= ~(1UL << k))
#define SET_K_TH_BIT(n,k)   (n |= 1UL << k)
#define SET_ALL_BITS -1

#define CLOCK_NOW         std::chrono::high_resolution_clock::now()
#define CURRENT_TIMESTAMP CLOCK_NOW.time_since_epoch().count()

#define ENABLE_LOG
#ifdef  ENABLE_LOG
#define LOG(...) click_chatter(__VA_ARGS__)
#else
#define LOG(...)
#endif

//#define ENABLE_DEBUG
#ifdef  ENABLE_DEBUG
#define DEBUG(...) click_chatter(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

/// definitions of assumptions
#define MAX_QUEUES         8
#define STATE_LEN          8
#define MAX_CHAIN_LEN      5
#define DEFAULT_OFFSET     76 // This value must be greater than 75
#define QUEUE_LEN          30
#define LOG_TABLE_RES_SIZE 10000

/// State and piggyback message definitions
typedef int16_t State[STATE_LEN];

typedef struct TimestampState {
    int64_t timestamp;
    State state;
} TimestampState;

typedef struct PiggybackState {
    int64_t last_commit;
    int64_t timestamp;
    State state;
} PiggybackState;

typedef PiggybackState PiggybackMessage[MAX_CHAIN_LEN];

/// Useful casting definitions
#define CAST_TO_BYTES(x)              reinterpret_cast<unsigned char *>(&x)
#define CAST_TO_STATE(x)              reinterpret_cast<State*>(x)
#define CAST_TO_TIMESTAMP_STATE(x)    reinterpret_cast<TimestampState*>(x)
#define CAST_TO_PIGGY_BACK_STATE(x)   reinterpret_cast<PiggybackState*>(x)
#define CAST_TO_PIGGY_BACK_MESSAGE(x) reinterpret_cast<PiggybackMessage*>(x)

#define CAST_AWAY_PACKET_DATA(p) const_cast<unsigned char *>(p->data())
#define CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p) reinterpret_cast<PiggybackMessage*>(CAST_AWAY_PACKET_DATA(p) + DEFAULT_OFFSET)
#define COPY_PIGGYBACK_MESSAGE(y, x) memcpy(y, x, sizeof(PiggybackMessage))

/// FTMB definitions
typedef int32_t SharedVarID;
typedef int64_t FTMBSeqNumber;
typedef FTMBSeqNumber VectorOfClocks[MAX_QUEUES];

typedef struct PacketAccessLog { /// We ignore p_i in FTMB's definition
    SharedVarID var_id;    /// v_j in FTMB's definition
    FTMBSeqNumber seq_num;       /// s_ij in FTMB's definition

    inline void set(SharedVarID var_id, FTMBSeqNumber seq_num) {
        this->var_id = var_id;
        this->seq_num = seq_num;
    }
} PacketAccessLog;

/// TF definitions
#define MAX_TF_CHUNK_SIZE 50
typedef TimestampState TFStates[MAX_TF_CHUNK_SIZE];

#define CAST_PACKET_TO_PAL(p) reinterpret_cast<PacketAccessLog*>(CAST_AWAY_PACKET_DATA(p) + DEFAULT_OFFSET)
//#define CAST_PACKET_TO_VOR(p) reinterpret_cast<VectorOfClocks*>(CAST_AWAY_PACKET_DATA(p) + DEFAULT_OFFSET)
#define CAST_PACKET_TO_VOR(p) reinterpret_cast<VectorOfClocks*>(CAST_AWAY_PACKET_DATA(p) + DEFAULT_OFFSET + sizeof(PacketAccessLog))

/// Some useful IP related defs
#define MAC_HEAD_SIZE    14
#define UDP_HEAD_OFFSET_AFTER_MAC_HEAD 1
#define DEFAULT_CRC 0

/// Util class to serialize, deserialize, encode, and decode states
class Util {
public:
    inline void init(State &s) {
        memset(s, 0, sizeof(State));
    }

    inline void init(TimestampState &s) {
        memset(&s, 0, sizeof(TimestampState));
    }

    inline void init(PiggybackState &s) {
        memset(&s, 0, sizeof(PiggybackState));
    }

    inline void init(PiggybackMessage &s) {
        memset(s, 0, sizeof(PiggybackMessage));
    }

    inline void copy(State& y, State& x) {
        memcpy(&y, &x, sizeof(State));
    }

    inline void copy(TimestampState& y, TimestampState& x) {
        memcpy(&y, &x, sizeof(TimestampState));
    }

    inline void copy(PiggybackState& y, PiggybackState& x) {
        memcpy(&y, &x, sizeof(PiggybackState));
    }

    void print(State &state) {
        for (auto i = 0; i < STATE_LEN; ++i) {
            LOG("%d: %d", i, state[i]);
        }//for
    }

    void print(PiggybackState &state) {
        LOG("last commit is %llu, timestamp is %llu", state.last_commit, state.timestamp);
        print(state.state);
    }

    void print(TimestampState &ft_state) {
        LOG("Timestamp: %llu", ft_state.timestamp);
        print(ft_state.state);
    }

    void print(PiggybackMessage &msg) {
        for (int i = 0; i < MAX_CHAIN_LEN; ++i) {
            LOG("\nState of middlebox %d", i);
            print(msg[i]);
        }//for
    }

    void random_state(State& s) {
        for (int i = 0; i < STATE_LEN; i++) {
            s[i] = click_random() % 100;
        }//for
    }

    void random_ts_state(TimestampState& ts_state) {
        ts_state.timestamp = CURRENT_TIMESTAMP;
        random_state(ts_state.state);
    }

    void random_piggyback(PiggybackState& pb_state) {
        pb_state.last_commit = CURRENT_TIMESTAMP;
        TimestampState ts_state;
        random_ts_state(ts_state);
        copy(pb_state.state, ts_state.state);
        pb_state.timestamp = ts_state.timestamp;
    }

    void random_message(PiggybackMessage& msg) {
        for (int i = 0; i < MAX_CHAIN_LEN; ++i) {
            random_piggyback(msg[i]);
        }//for
    }

    static inline void nsleep(long dur_ns) {
        auto start = CLOCK_NOW;
        while (std::chrono::duration_cast<std::chrono::nanoseconds>(CLOCK_NOW - start).count() < dur_ns)
            ;
    }

    static inline bool npassed(std::chrono::time_point<std::chrono::high_resolution_clock>& tp, long dur_ns) {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(CLOCK_NOW - tp).count() >= dur_ns;
    }

    static inline long dummy_loop_count(long ns, double error) {
        long lb = ns - error * ns;
        long ub = ns + error * ns;

        long count, delay, min, max;

        min = 0;
        max = 100000;

        volatile long ii;

        auto start = CLOCK_NOW;

        std::vector<long> delays;

        while (true) {
            count = (min + max) / 2;
            long delays = 0;
            for (auto i = 0; i < 100; ++i) {
                // Measurement
                ii = 0;
                start = CLOCK_NOW;
                while (ii < count) ++ii;
                delay = std::chrono::duration_cast<std::chrono::nanoseconds>(CLOCK_NOW - start).count();

                delays += delay;
            }//while

            auto avg_delay = delays / 100.0;

            if (avg_delay < ub &&
                avg_delay > lb)
                break;

            if (avg_delay > ub)
                max = count;

            if (avg_delay < lb)
                min = count;
        }//while

        return count;
    }

    static inline long find_dummy_loop_count(long ns, double error = 0.01) {
        auto mean = 0.0;
        auto loop = 100;
        for (int i = 0; i < loop; ++i)
            mean += dummy_loop_count(ns, error);
        mean /= (double)loop;

        return mean;
    }

    #pragma GCC push_options
    #pragma GCC optimize("O0")
    static inline void dummy_loop(volatile long cnt) {
        while (cnt > 0) cnt--;
    }
    #pragma GCC pop_options

    static inline void no_op_pkt_destructor(unsigned char* buf, size_t sz, void* argument) {
        // No operation
    }
};

#endif //FTSFC_DEFS_HH
