#pragma once

#include <click/config.h>
#include <click/element.hh>
#include <vector>
#include "FTTypes.hh"

CLICK_DECLS

/**
 * This class filter the packets that do not match given vlan_ids.
 * The vlan_ids are given in the click configuration as a list of parameters.
 */
class FTFilterElement: public Element {
private:
    std::vector<VLANId> _vlan_ids;

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