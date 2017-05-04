#include <click/config.h>
#include <click/args.hh>
#include <clicknet/ether.h>
#include "FTFilterElement.hh"

CLICK_DECLS

FTFilterElement::FTFilterElement() { }

FTFilterElement::~FTFilterElement() { }

int FTFilterElement::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh)
                .read(CONF_VLAN_ID, BoundedIntArg(0, 0xFFF), _vlan_id)
                .complete() < 0)
        return -1;

    return 0;
}

Packet *FTFilterElement::simple_action(Packet *p) {
    const click_ether_vlan *vlan = reinterpret_cast<const click_ether_vlan *>(p->data());
    VLANId vlan_id = (ntohs(vlan->ether_vlan_tci) & 0xFFF);

    if (vlan_id != _vlan_id)
        return 0;

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTFilterElement)