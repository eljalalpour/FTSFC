#pragma once

#include "defs.hh"
//#include <map>
#include <queue>
#include <click/config.h>
#include <click/element.hh>
#include <click/standard/storage.hh>

CLICK_DECLS

///
/// Because of the same argument as of \texttt{Forwarder}'s,
/// \texttt{Buffer} threads can execute independently.
///

#define TO_OUTSIDE_WORLD 0
#define TO_FORWARDER   1
#define INIT_TIMESTAMPS_SIZE 1000000

class Buffer : public Element, public Storage {
private:
    int _chain_len;

//    std::map<int64_t, Packet *> _packets;
    std::queue<int64_t> _timestamps;
    Packet* volatile * _q;

    inline void _release(int64_t);

    inline Packet* deq() {
        Storage::index_type h = head(), t = tail();
        if (h != t) {
            Packet *p = _q[h];
            set_head(next_i(h));
            assert(p);
            return p;
        }//if
        else {
            return 0;
        }//else
    }

public:
    Buffer ();

    ~Buffer();

    int initialize(ErrorHandler*) CLICK_COLD;

    const char *class_name() const { return "Buffer"; }

    const char *port_count() const { return PORTS_1_2; }

    const char *processing() const { return AGNOSTIC; }

    void push(int, Packet*);

    int configure(Vector<String> &, ErrorHandler *);

    size_t length();
};

CLICK_ENDDECLS