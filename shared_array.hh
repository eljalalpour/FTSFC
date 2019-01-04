#ifndef LOCK_FREE_ARRAY_HH
#define LOCK_FREE_ARRAY_HH

#include <click/config.h>
#include <click/element.hh>
#include "shared_locks.hh"

#define DEFAULT_SIZE         8
#define DEFAULT_HANDLER_SIZE 8

CLICK_DECLS

class SharedArray : public Element {
public:
    enum SharingLevel {
        ThreadSharing1 = 1, // no sharing
        ThreadSharing2 = 2, // 2 threads share the same counter
        ThreadSharing4 = 4, // 4 threads share the same counter
        ThreadSharing8 = 8, // all threads share the same counter
    };

    SharedArray ();

    ~SharedArray();

    const char *class_name() const { return "SharedArray"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *);

    int configure(Vector<String>&, ErrorHandler*) CLICK_COLD;

    void add_handlers();

    inline void increment(size_t);

    inline int read(size_t);

private:
    int _array[DEFAULT_SIZE];
    int _sharing_level;
    String _shared_element_name;
    SharedLocks *_shared_locks;

    void _init_shared_locks();

    static String read_handler(Element*, void*);
};

void SharedArray::increment(size_t index) {
    size_t lock_index = index % (DEFAULT_SIZE / _sharing_level);
    _shared_locks->lock(lock_index);
    ++_array[index];
    _shared_locks->unlock(lock_index);
}

int SharedArray::read(size_t index) {
    size_t lock_index = index % (DEFAULT_SIZE / _sharing_level);
    _shared_locks->lock(lock_index);
    auto val = _array[index];
    _shared_locks->unlock(lock_index);

    return val;
}

CLICK_ENDDECLS
#endif