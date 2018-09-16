#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>

CLICK_DECLS

///
/// Because of the log propagation invariant and \texttt{Forwarder} being stateless,
/// its threads do not need any concurrency control.
/// They independently maintain a memory for and decode the piggybak message received
/// from \texttt{Buffer} and encode this memory to the incoming packets.
///

class Forwarder : public Element {
private:
    int _chain_len;

    Util _util;
    PiggybackMessage _msg;

public:
    Forwarder ();

    ~Forwarder();

    const char *class_name() const { return "Forwarder"; }

    const char *port_count() const { return PORTS_2_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &, ErrorHandler *);

    void push(int, Packet*);
};

CLICK_ENDDECLS