#include <click/config.h>
#include <click/args.hh>
#include <clicknet/ether.h>
#include "FTPassElement.hh"
CLICK_DECLS

FTPassElement::FTPassElement() { }

FTPassElement::~FTPassElement() { }

void FTPassElement::push(int port, Packet *p) {
    LOG("------------------------------");
    LOG("Begin FTPassElement - push");

    const click_ether_vlan *vlan = reinterpret_cast<const click_ether_vlan *>(p->data());
    VLANId vlan_id = (ntohs(vlan->ether_vlan_tci) & 0xFFF);
    LOG("Port: %d, VLAN_ID: %d", port, vlan_id);

    LOG("End FTPassElement - push");
    LOG("------------------------------");
    //TODO: change port back to zero
    output(0).push(p);
}

Packet* FTPassElement::simple_action(Packet* p) {
    LOG("------------------------------");
    LOG("Begin FTPassElement - Simple action");

    const click_ether_vlan *vlan = reinterpret_cast<const click_ether_vlan *>(p->data());
    VLANId vlan_id = (ntohs(vlan->ether_vlan_tci) & 0xFFF);
    LOG("VLAN_ID: %d", vlan_id);

    LOG("End FTPassElement - Simple action");
    LOG("------------------------------");
    return p;
}

Packet* FTPassElement::pull(int port) {
    LOG("------------------------------");
    LOG("Begin FTPassElement - pull");

    Packet *p = input(port).pull();

    const click_ether_vlan *vlan = reinterpret_cast<const click_ether_vlan *>(p->data());
    VLANId vlan_id = (ntohs(vlan->ether_vlan_tci) & 0xFFF);

    LOG("VLAN_ID: %d", vlan_id);

    //TODO: change port back to zero
    LOG("End FTPassElement - pull");
    LOG("------------------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTPassElement)