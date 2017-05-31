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
    output(port).push(p);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTPassElement)