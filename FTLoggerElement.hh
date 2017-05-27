#pragma once

#include <click/config.h>
#include <click/element.hh>
#include <vector>
#include <map>
#include <string>
#include "FTAppenderElement.hh"

CLICK_DECLS

#define LOG_PATH_CONF "PATH"
#define COUNT_CONF    "COUNT"

/**
 * This element recieves a packet from two input ports and pass packet to a single output
 */
class FTLoggerElement: public Element {
private:
    static map<FTPacketId, FTTimestamp> _pkt_time;
    static String _path;
    static long long _count;
    static void _signal_handler(int signal);
    static void _write_to_file();

public:
    FTLoggerElement();
    ~FTLoggerElement();

    int configure(Vector<String> &conf, ErrorHandler *errh);

    const char *class_name() const { return "FTLoggerElement"; }
    const char *port_count() const { return PORTS_1_1; }
    const char *processing() const { return AGNOSTIC; }

    void push(int, Packet *p);
};

CLICK_ENDDECLS