#include <click/config.h>
#include <click/args.hh>
#include <clicknet/ether.h>
#include "FTPassElement.hh"
CLICK_DECLS

FTPassElement::FTPassElement() { }

FTPassElement::~FTPassElement() { }

void FTPassElement::push(int port, Packet *p) {
    click_chatter("------------------------------");
    click_chatter("Begin FTPassElement - push");

    const click_ether_vlan *vlan = reinterpret_cast<const click_ether_vlan *>(p->data());
    VLANId vlan_id = (ntohs(vlan->ether_vlan_tci) & 0xFFF);
    click_chatter("VLAN_ID: %d", vlan_id);

    click_chatter("End FTPassElement - push");
    click_chatter("------------------------------");
    //TODO: change port back to zero
    output(0).push(p);
}

Packet* FTPassElement::simple_action(Packet* p) {
    click_chatter("------------------------------");
    click_chatter("Begin FTPassElement - Simple action");

    const click_ether_vlan *vlan = reinterpret_cast<const click_ether_vlan *>(p->data());
    VLANId vlan_id = (ntohs(vlan->ether_vlan_tci) & 0xFFF);
    click_chatter("VLAN_ID: %d", vlan_id);

    click_chatter("End FTPassElement - Simple action");
    click_chatter("------------------------------");
    return p;
}

Packet* FTPassElement::pull(int port) {
    click_chatter("------------------------------");
    click_chatter("Begin FTPassElement - pull");

    Packet *p = input(port).pull();

    const click_ether_vlan *vlan = reinterpret_cast<const click_ether_vlan *>(p->data());
    VLANId vlan_id = (ntohs(vlan->ether_vlan_tci) & 0xFFF);

    click_chatter("VLAN_ID: %d", vlan_id);

    //TODO: change port back to zero
    click_chatter("End FTPassElement - pull");
    click_chatter("------------------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTPassElement)