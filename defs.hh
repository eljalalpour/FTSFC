#ifndef FTSFC_DEFS_HH
#define FTSFC_DEFS_HH

/// Assumptions:
///     - The state of a middlebox is a FIXED int array with 8 values (32 bytes)
///     - The piggyback message always contains the piggback state of 5 middleboxes
///     - Middleboxes must not modify packets

#include <list>
#include <unordered_map>
#include <chrono>
#include <cstdlib>
#include <click/packet.hh>
#include <click/glue.hh>
#include <vector>

using std::vector;

/// Useful  definitions
#define PORTS_2_1 "2/1"
#define PORTS_1_2 "1/2"
#define PORTS_2_2 "2/2"

#define CURRENT_TIMESTAMP std::chrono::high_resolution_clock::now().time_since_epoch().count()

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
#define STATE_LEN          8
#define MAX_CHAIN_LEN      5
#define DEFAULT_OFFSET     76 // This value must be greater than 75
#define QUEUE_LEN          30
#define LOG_TABLE_MAX_SIZE 10000
#define LOG_TABLE_CON_SIZE 34

/// State and piggyback message definitions
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
} PiggybackState;

typedef PiggybackState PiggybackMessage[MAX_CHAIN_LEN];

template <typename value_type>
class FTVector {
private:
    std::vector<value_type> _elems;
    size_t _head;
    size_t _tail;
    size_t _max_size;
    size_t _size;

    inline void _pushed_back () {
        ++_size;
        _tail = (_tail + 1) % _max_size;
    }

    inline size_t _convert_index (const size_t& i) {
        return (_head + i) % _max_size;
    }

public:
    FTVector(size_t max_size):
            _max_size(max_size),
            _size(0),
            _head(0),
            _tail(0),
            _elems(max_size, value_type()) { }

    FTVector(): FTVector(LOG_TABLE_MAX_SIZE) { }

    void push_back(const value_type& val) {
        if (full()) {
            throw "No empty space!";
        }//if

        if (empty()) {
            _head = 0;
            _tail = 0;
        }//if

        _elems[_tail] = val;
        _pushed_back();
    }

    void push_back (value_type&& val) {
        if (full()) {
            throw "No empty space!";
        }//if

        if (empty()) {
            _head = 0;
            _tail = 0;
        }//if

        _elems[_tail] = val;
        _pushed_back();
    }

    inline size_t size() {
        return _size;
    }

    void erase(size_t count) {
        if (count > _size) {
            throw "Big vector!";
        }//if

        _head = (_head + count) % _max_size;
        _size -= count;
    }

    inline bool empty() {
        return _size == 0;
    }

    inline bool full() {
        return _size == _max_size;
    }

    value_type &operator[](const size_t i) {
        if( i >= _size ) {
            throw "Index out of bound!";
        }
        return _elems[_convert_index(i)];
    }

    typename vector<value_type>::iterator iterator(size_t index) {
        return (_elems.begin() + _convert_index(index));
    }//if

    typename vector<value_type>::iterator rbegin() {
        if (empty())
            return _elems.end();
        return (_elems.begin() + _convert_index(_size - 1));
    }//if
};

typedef FTVector<TimestampState> TimestampStateList;

typedef TimestampStateList LogTable[MAX_CHAIN_LEN];

typedef TimestampState CommitMemory[MAX_CHAIN_LEN];

/// Useful casting definitions
#define CAST_TO_BYTES(x)              reinterpret_cast<unsigned char *>(&x)
#define CAST_TO_STATE(x)              reinterpret_cast<State*>(x)
#define CAST_TO_TIMESTAMP_STATE(x)    reinterpret_cast<TimestampState*>(x)
#define CAST_TO_PIGGY_BACK_STATE(x)   reinterpret_cast<PiggybackState*>(x)
#define CAST_TO_PIGGY_BACK_MESSAGE(x) reinterpret_cast<PiggybackMessage*>(x)

#define CAST_AWAY_PACKET_DATA(p) const_cast<unsigned char *>(p->data())
#define CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p) reinterpret_cast<PiggybackMessage*>(CAST_AWAY_PACKET_DATA(p) + DEFAULT_OFFSET)

#define COPY_PIGGYBACK_MESSAGE(y, x) memcpy(y, x, sizeof(PiggybackMessage))

/// Util class to serialize, deserialize, encode, and decode states
class Util {
public:
    inline void init(State &s) {
        memset(&s, 0, sizeof(State));
    }

    inline void init(TimestampState &s) {
        memset(&s, 0, sizeof(TimestampState));
    }

    inline void init(PiggybackState &s) {
        memset(&s, 0, sizeof(PiggybackState));
    }

    inline void init(PiggybackMessage &s) {
        memset(&s, 0, sizeof(PiggybackMessage));
    }

    inline void init(CommitMemory &s) {
        memset(&s, 0, sizeof(CommitMemory));
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

    inline void copy(TimestampState& y, PiggybackState& x) {
        y.timestamp = x.timestamp;
        copy(y.state, x.state);
    }

//    inline void copy(PiggybackMessage y, PiggybackMessage x) {
//        memcpy(y, x, sizeof(PiggybackMessage));
//    }
//
//    inline void copy(PiggybackMessage y, PiggybackMessage* x) {
//        memcpy(y, *x, sizeof(PiggybackMessage));
//    }
//
//    inline void serialize(const State &s, unsigned char *ser) {
//        memcpy(ser, &s, sizeof(State));
//    }
//
//    inline void deserialize(State &s, const unsigned char* ser) {
//        memcpy(&s, ser, sizeof(State));
//    }
//
//    inline void serialize(const TimestampState &s, unsigned char *ser) {
//        memcpy(ser, &s, sizeof(TimestampState));
//    }
//
//    inline void deserialize(TimestampState &s, const unsigned char* ser) {
//        memcpy(&s, ser, sizeof(TimestampState));
//    }
//
//    inline void serialize(const PiggybackState &s, unsigned char *ser) {
//        memcpy(ser, &s, sizeof(PiggybackState));
//    }
//
//    inline void deserialize(PiggybackState &s, const unsigned char* ser) {
//        memcpy(&s, ser, sizeof(PiggybackState));
//    }
//
//    inline void serialize(const PiggybackMessage &s, unsigned char *ser) {
//        memcpy(ser, &s, sizeof(PiggybackMessage));
//    }
//
//    inline void deserialize(PiggybackMessage &s, const unsigned char* ser) {
//        memcpy(&s, ser, sizeof(PiggybackMessage));
//    }
//
//    inline void encode(PiggybackMessage& s, Packet* p) {
//        // Cast away and point to the offset where PiggybackMessage is encoded,
//        // then encode PiggybackMessage
////        serialize(s, CAST_AWAY_PACKET_DATA(p) + DEFAULT_OFFSET);
//        PiggybackMessage* t = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
//        copy(*t, s);
//    }
//
//    inline void decode(PiggybackMessage& s, const Packet* p) {
//        PiggybackMessage* t = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
//        copy(s, *t);
////        deserialize(s, p->data() + DEFAULT_OFFSET);
//    }

    void print(State &state) {
        for (auto i = 0; i < STATE_LEN; ++i) {
            LOG("%d: %d", i, state[i]);
        }//for
    }

    void print(TimestampState &ft_state) {
        LOG("Timestamp: %llu", ft_state.timestamp);
        print(ft_state.state);
    }

    void print(PiggybackState &state) {
        LOG("ack is %d, last commit is %llu, timestamp is %llu", state.ack, state.last_commit, state.timestamp);
        print(state.state);
    }

    void print(PiggybackMessage &msg) {
        for (auto i = 0; i < MAX_CHAIN_LEN; ++i) {
            LOG("\nState of middlebox %d", i);
            print(msg[i]);
        }//for
    }

    void print(PiggybackMessage *msg) {
        for (int i = 0; i < MAX_CHAIN_LEN; ++i) {
            LOG("\nState of middlebox %d", i);
            print(*msg[i]);
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
        pb_state.ack = click_random() % MAX_CHAIN_LEN;
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
};



//class TimestampMeasure {
//public:
//    TimestampMeasure() : _usec_beg(0), _usec_accum(0), _count(0) { }
//
//    inline uint64_t count() {
//        return _count;
//    }
//
//    inline double time() {
//        return _usec_accum;
//    }
//
//    inline double average_time() {
//        return _usec_accum / _count;
//    }
//
//    inline String report() {
//        return String(_count) + ","
//            + String(_usec_accum) + ","
//            + String(_usec_accum / _count);
//    }
//
//protected:
//    void beg() {
//        _usec_beg = Timestamp::now();
//    }
//
//    void end() {
//        _usec_accum += Timestamp::now().doubleval() - _usec_beg;
//        _count++;
//    }
//
//private:
//    double _usec_beg;
//    double _usec_accum;
//    uint64_t _count;
//};

#endif //FTSFC_DEFS_HH
