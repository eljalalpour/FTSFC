#include <click/config.h>
#include <click/args.hh>
#include "FTPassElement.hh"

CLICK_DECLS

FTPassElement::FTPassElement() { }

FTPassElement::~FTPassElement() { }

void FTPassElement::push(int port, Packet *p) {
//    output(0).push(p);
    click_chatter("------------------------------");
    click_chatter("Begin FTPassElement");

    output(0).push(p);

    click_chatter("End FTPassElement");
    click_chatter("------------------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTPassElement)