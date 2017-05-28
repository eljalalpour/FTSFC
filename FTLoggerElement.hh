#pragma once

#include <click/config.h>
#include <click/element.hh>
#include <limits>
#include "FTTypes.hh"
CLICK_DECLS

#define LOG_FILE_CONF "PATH"
#define COUNT_CONF    "COUNT"
#define COUNT_DEF_VAL std::numeric_limits<double>::infinity()

/**
 * This element recieves a packet from two input ports and pass packet to a single output
 */
class FTLoggerElement: public Element {
private:
    map<FTPacketId, FTTimestamp> _pkt_time;
    String _log_file;
    long long _count;
    bool _file_created;

public:
    FTLoggerElement();
    ~FTLoggerElement();

    void write_to_file();

    int configure(Vector<String> &conf, ErrorHandler *errh);

    const char *class_name() const { return "FTLoggerElement"; }
    const char *port_count() const { return PORTS_1_1; }
    const char *processing() const { return AGNOSTIC; }

    void push(int, Packet *p);
};

CLICK_ENDDECLS