#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include <deque>

CLICK_DECLS

#define ENABLE_MULTI_THREADING_USING_MUTEX 1
//#define ENABLE_MULTI_THREADING_USING_FLAG_LOCK 1
//#define ENABLE_MULTI_THREADING_USING_ELIDED_LOCK 1
//#define ENABLE_MULTI_THREADING_USING_FINE_GRAINED_LOCKS 1
//#define ENABLE_MULTI_THREADING_USING_FINE_GRAINED_ELIDED_LOCKS 1


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

#ifdef ENABLE_MULTI_THREADING_USING_MUTEX
    std::deque<std::mutex> reader_lockers;
    std::deque<std::mutex> writer_lockers;
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FLAG_LOCK
    std::deque<flag_spin_lock> reader_lockers;
    std::deque<flag_spin_lock> writer_lockers;
#endif

#ifdef ENABLE_MULTI_THREADING_USING_ELIDED_LOCK
    std::deque<elided_spin_lock> reader_lockers;
    std::deque<elided_spin_lock> writer_lockers;
#endif
    inline void extend_readers_size(size_t);

    inline void extend_writers_size(size_t);

    inline void extend_size(size_t);
};

void SharedLocks::extend_readers_size(size_t new_size) {
    for (auto i = reader_lockers.size(); i < new_size; ++i)
        reader_lockers.emplace_back();
}

void SharedLocks::extend_writers_size(size_t new_size) {
    for (auto i = writer_lockers.size(); i < new_size; ++i)
        writer_lockers.emplace_back();
}

void SharedLocks::extend_size(size_t new_size) {
    extend_readers_size(new_size);
    extend_writers_size(new_size);
}

CLICK_ENDDECLS