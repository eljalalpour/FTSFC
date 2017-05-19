#include <click/config.h>
#include <click/args.hh>
#include <clicknet/ether.h>
#include "FTFilterElement.hh"

CLICK_DECLS

FTFilterElement::FTFilterElement(): _all(0), _passed(0) { };

FTFilterElement::~FTFilterElement() { }

int FTFilterElement::configure(Vector<String> &conf, ErrorHandler *errh) {
    VLANId vlanId;
    BoundedIntArg parser(0, 0xFFF);
    for (int i = 0; i < conf.size(); ++i) {
        parser.parse(conf[i], vlanId);
        _vlan_ids.push_back(vlanId);
        click_chatter("VLAN ID: %d", vlanId);
    }//for

    return 0;
}

Packet *FTFilterElement::simple_action(Packet *p) {
    click_chatter("--------------------");
    click_chatter("In FTFilter Element");

    _all++;
    const click_ether_vlan *vlan = reinterpret_cast<const click_ether_vlan *>(p->data());
    VLANId vlan_id = (ntohs(vlan->ether_vlan_tci) & 0xFFF);

    bool found = false;
    for (auto it = _vlan_ids.begin(); !found && it != _vlan_ids.end(); ++it) {
        found = ((*it) == vlan_id);
    }//for

    if (found)
        _passed++;

    click_chatter("All packets: %d, passed packets: %d", _all, _passed);
    click_chatter("--------------------");

    if (!found)
        return 0;

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTFilterElement)