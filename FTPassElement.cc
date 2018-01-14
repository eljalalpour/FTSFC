#include <click/config.h>
#include <click/args.hh>
#include <clicknet/ether.h>
#include "FTPassElement.hh"
CLICK_DECLS

FTPassElement::FTPassElement() { }

FTPassElement::~FTPassElement() { }

void FTPassElement::push(int port, Packet *p) {
    output(port).push(p);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTPassElement)