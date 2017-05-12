#include <click/config.h>
#include <click/args.hh>
#include "FTPassElement.hh"

CLICK_DECLS

FTPassElement::FTPassElement() { }

FTPassElement::~FTPassElement() { }

void FTPassElement::push(int, Packet *p) {
    output(0).push(p);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTFilterElement)