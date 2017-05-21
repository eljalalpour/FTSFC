#include <click/config.h>
#include <click/args.hh>
#include "FTPassElement.hh"
CLICK_DECLS

FTPassElement::FTPassElement() { }

FTPassElement::~FTPassElement() { }

void FTPassElement::push(int port, Packet *p) {
//    output(0).push(p);
    LOG("------------------------------");
    LOG("Begin FTPassElement");

    //TODO: change port to zero
    output(port).push(p);

    LOG("End FTPassElement");
    LOG("------------------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTPassElement)