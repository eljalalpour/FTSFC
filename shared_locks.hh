#pragma once

#include "defs.hh"
#include "elided_spin_lock.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include <deque>

CLICK_DECLS

#define ENABLE_MULTI_THREADING_USING_MUTEX 1
//#define ENABLE_MULTI_THREADING_USING_FLAG_LOCK 1
//#define ENABLE_MULTI_THREADING_USING_ELIDED_LOCK 1

#ifdef ENABLE_MULTI_THREADING_USING_MUTEX
typedef std::mutex Locker;
#endif

#ifdef ENABLE_MULTI_THREADING_USING_FLAG_LOCK
typedef flag_spin_lock Locker;
#endif

#ifdef ENABLE_MULTI_THREADING_USING_ELIDED_LOCK
typedef elided_spin_lock Locker;
#endif

typedef std::deque<Locker> Lockers;

#define LOCK(locker)   if (locker != nullptr) locker->lock()
#define UNLOCK(locker) if (locker != nullptr) locker->unlock()

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
    inline void extend(size_t);

    inline void lock(size_t);

    inline void unlock(size_t);

    inline Locker* locker_ptr(size_t);
private:
    std::mutex _my_mutex;
    Lockers _lockers;
};

void SharedLocks::extend(size_t new_size) {
    if (new_size > _lockers.size()) {
        std::lock_guard<std::mutex> guard(_my_mutex);
        for (auto i = _lockers.size(); i < new_size; ++i) {
            _lockers.emplace_back();
        }//for
    }//if
}

void SharedLocks::lock(size_t index) {
    _lockers[index].lock();
}

void SharedLocks::unlock(size_t index) {
    _lockers[index].unlock();
}

Locker* SharedLocks::locker_ptr(size_t index) {
    return &_lockers[index];
}

CLICK_ENDDECLS