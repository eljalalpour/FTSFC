#pragma once

#include "defs.hh"
#include "elided_spin_lock.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include <shared_mutex>
#include <deque>

CLICK_DECLS

#define ENABLE_MULTI_THREADING_USING_MUTEX 1
//#define ENABLE_MULTI_THREADING_USING_FLAG_LOCK 1
//#define ENABLE_MULTI_THREADING_USING_ELIDED_LOCK 1
//#define ENABLE_MULTI_THREADING_USING_FINE_GRAINED_LOCKS 1
//#define ENABLE_MULTI_THREADING_USING_FINE_GRAINED_ELIDED_LOCKS 1


#ifdef ENABLE_MULTI_THREADING_USING_MUTEX
typedef std::deque<std::mutex> ReaderLockers;
typedef std::deque<std::mutex> WriterLockers;

typedef std::deque<std::shared_lock<std::mutex>> ReaderLocks;
typedef std::deque<std::unique_lock<std::mutex>> WriterLocks;
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FLAG_LOCK
typedef std::deque<flag_spin_lock> ReaderLockers;
typedef std::deque<flag_spin_lock> WriterLockers;

typedef std::deque<std::unique_lock<flag_spin_lock>> WriterLocks;
typedef std::deque<std::shared_lock<flag_spin_lock>> ReaderLocks;
#endif

#ifdef ENABLE_MULTI_THREADING_USING_ELIDED_LOCK
typedef std::deque<elided_spin_lock> ReaderLockers;
typedef std::deque<elided_spin_lock> WriterLockers;

typedef std::deque<std::unique_lock<elided_spin_lock>> WriterLocks;
typedef std::deque<std::shared_lock<elided_spin_lock>> ReaderLocks;
#endif

class SharedLocks : public Element {
public:

    SharedLocks ();

    ~SharedLocks();

    const char *class_name() const { return "SharedLocks"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    int configure(Vector<String> &, ErrorHandler *);

public:
    inline void extend_readers_size(size_t);
    inline void extend_writers_size(size_t);
    inline void extend_size(size_t);

    inline void lock_reader(size_t);
    inline void lock_writer(size_t);

    inline void unlock_reader(size_t);
    inline void unlock_writer(size_t);

private:
    std::mutex _my_mutex;

    ReaderLockers _reader_lockers;
    WriterLockers _writer_lockers;

    ReaderLocks _reader_locks;
    WriterLocks _writer_locks;

    template<typename LockersType, typename LocksType>
    inline void _extend(LockersType& lockers, LocksType& locks, size_t new_size) {
        if (new_size > lockers.size()) {
            std::lock_guard<std::mutex> guard(_my_mutex);
            for (auto i = lockers.size(); i < new_size; ++i) {
                lockers.emplace_back();
                locks.emplace_back(lockers.back());
            }//for
        }//if
    }
};

void SharedLocks::extend_readers_size(size_t new_size) {
    _extend(_reader_lockers, _reader_locks, new_size);
}

void SharedLocks::extend_writers_size(size_t new_size) {
    _extend(_writer_lockers, _writer_locks, new_size);
}

void SharedLocks::extend_size(size_t new_size) {
    extend_readers_size(new_size);
    extend_writers_size(new_size);
}

void SharedLocks::lock_reader(size_t index) {
    _reader_locks[index].lock();
}

void SharedLocks::lock_writer(size_t index) {
    _writer_locks[index].lock();
}

void SharedLocks::unlock_reader(size_t index) {
    _reader_locks[index].unlock();
}

void SharedLocks::unlock_writer(size_t index) {
    _writer_locks[index].unlock();
}

CLICK_ENDDECLS