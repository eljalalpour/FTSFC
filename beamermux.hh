#pragma once

#include <click/config.h>
#include <click/element.hh>
#include <click/ipaddress.hh>

CLICK_DECLS

class BeamerMux: Element {
public:
    BeamerMux();

    ~BeamerMux();

    const char *class_name() const { return "BeamerMux"; }

    const char *port_count() const { return "1/1"; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);

private:
    IPAddress vip;

    Packet *handleUDP(Packet *);
};


CLICK_ENDDECLS