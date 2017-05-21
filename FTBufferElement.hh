#pragma once

#include <click/config.h>
#include <click/element.hh>
#include "FTTypes.hh"

#define TO_OUTSIDE_WORLD 0
#define TO_CHAIN_BEGIN  1
#define FT_BUFFER_ELEMENT_CLASS_NAME "FTBufferElement"

CLICK_DECLS

class FTBufferElement: public Element {
private:
    map<FTPacketId, Packet * > _packets;
    int _all;
    int _released;
    map<FTPacketId, int> _checked_to_release;

public:
    FTBufferElement();
    ~FTBufferElement();

    const char *class_name() const { return "FTBufferElement"; }

    const char *port_count() const { return PORTS_1_2; }
    const char *processing() const { return AGNOSTIC; }

    //TODO: make sure that the connection type is push
    void push(int, Packet *p);
};

CLICK_ENDDECLS