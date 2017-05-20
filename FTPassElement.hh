#pragma once

#include <click/config.h>
#include <click/element.hh>
#include <vector>
#include "FTTypes.hh"

CLICK_DECLS

/**
 * This element recieves a packet from two input ports and pass packet to a single output
 */
class FTPassElement: public Element {
public:
    FTPassElement();
    ~FTPassElement();

    const char *class_name() const { return "FTPassElement"; }
    //TODO: change PORTS_2_2 to PORTS_2_1
    const char *port_count() const { return PORTS_2_2; }
    const char *processing() const { return AGNOSTIC; }

    void push(int, Packet *p);
};

CLICK_ENDDECLS