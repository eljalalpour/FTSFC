#pragma once

#include <click/config.h>
#include <click/element.hh>
#include <limits>
#include "FTTypes.hh"
CLICK_DECLS

#define IP_OFFSET     "OFFSET"
#define LOG_FILE_CONF "PATH"
#define COUNT_CONF    "COUNT"
#define COUNT_DEF_VAL LLONG_MAX
#define IP_OFFSET_DEF_VAL 14

/**
 * This element recieves a packet from two input ports and pass packet to a single output
 */
class FTLoggerElement: public Element {
private:
    unordered_map<FTPacketId, FTTimestamp> _pkt_time;
    String _log_file;
    long long _count;
    int _ip_offset;
    bool _file_created;

public:
    FTLoggerElement();
    ~FTLoggerElement();

    void write_to_file();

    int configure(Vector<String> &conf, ErrorHandler *errh);

    const char *class_name() const { return "FTLoggerElement"; }
    const char *port_count() const { return PORTS_1_1; }
    const char *processing() const { return AGNOSTIC; }

    Packet* simple_action(Packet* p);
    size_t num_packets();
};

CLICK_ENDDECLS