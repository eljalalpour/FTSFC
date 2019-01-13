#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <cstring>

CLICK_DECLS

class FTMBOutputLogger : public Element {
public:
    FTMBOutputLogger ();

    ~FTMBOutputLogger();

    const char *class_name() const { return "FTMBOutputLogger"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

private:
    int16_t _queue;
    PacketAccessLog _last_pal;
    VectorOfClocks _last_vor;

//    inline void _process_vor(Packet*);
//    inline void _process_pal(Packet*);
    inline void _process_extra(Packet*);
};

//void FTMBOutputLogger::_process_vor(Packet* p) {
//    std::memcpy(_last_vor, CAST_PACKET_TO_VOR(p), sizeof(VectorOfClocks));
//}
//
//void FTMBOutputLogger::_process_pal(Packet* p) {
//    std::memcpy(&_last_pal, CAST_PACKET_TO_PAL(p), sizeof(PacketAccessLog));
//}

void FTMBOutputLogger::_process_extra(Packet* p) {
    std::memcpy(&_last_pal, CAST_PACKET_TO_PAL(p), sizeof(PacketAccessLog));
    std::memcpy(_last_vor, CAST_PACKET_TO_VOR(p), sizeof(VectorOfClocks));
}

CLICK_ENDDECLS