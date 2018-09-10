#pragma once

#include "defs.hh"
#include <map>
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

///
/// Because of the same argument as of \texttt{Forwarder}'s,
/// \texttt{Buffer} threads can execute independently.
///

#define TO_OUTSIDE_WORLD 0
#define TO_CHAIN_BEGIN   1

class Buffer : public Element {
private:
    volatile PiggybackMessage* _msg;

    map<int64_t, Packet *> _packets;

    inline int64_t _last_timestamp(PiggybackMessage&);

    inline int64_t _last_commit_timestamp(PiggybackMessage&);

    inline void _release(int64_t);

public:
    Buffer ();

    ~Buffer();

    const char *class_name() const { return "Buffer"; }

    const char *port_count() const { return PORTS_1_2; }

    const char *processing() const { return AGNOSTIC; }

    void push(int, Packet*);
};

CLICK_ENDDECLS