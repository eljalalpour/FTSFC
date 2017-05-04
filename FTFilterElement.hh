#pragma once

#include <click/config.h>
#include <click/element.hh>
#include "FTTypes.hh"

#define CONF_VLAN_ID "VLAN_ID"

CLICK_DECLS

/**
 * This class filter the packets that do not match a given vlan_id.
 * The vlan_id is given in the click configuration file
 */
class FTFilterElement: public Element {
private:
    VLANId _vlan_id;

public:
    FTFilterElement();
    ~FTFilterElement();

    const char *class_name() const { return "FTFilterElement"; }
    const char *port_count() const { return PORTS_1_1; }
    const char *processing() const { return AGNOSTIC; }
    int configure(Vector<String> &, ErrorHandler *);

    //TODO: make sure that the connection type is push
    Packet * simple_action(Packet *p);
};

CLICK_ENDDECLS