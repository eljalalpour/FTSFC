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
#define TO_FORWARDER   1

class Buffer : public Element {
private:
    int _chain_len;

    std::map<int64_t, Packet *> _packets;

    inline void _release(int64_t);

public:
    Buffer ();

    ~Buffer();

    const char *class_name() const { return "Buffer"; }

    const char *port_count() const { return PORTS_1_2; }

    const char *processing() const { return AGNOSTIC; }

    void push(int, Packet*);

    int configure(Vector<String> &, ErrorHandler *);

    size_t length();
};

CLICK_ENDDECLS