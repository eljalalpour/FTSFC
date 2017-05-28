#include <click/config.h>
#include <click/args.hh>
#include "FTPassElement.hh"
CLICK_DECLS

FTPassElement::FTPassElement() { }

FTPassElement::~FTPassElement() { }

void FTPassElement::push(int port, Packet *p) {
    LOG("------------------------------");
    LOG("Begin FTPassElement");

    //TODO: change port back to zero
    output(0).push(p);

    LOG("End FTPassElement");
    LOG("------------------------------");
}

Packet* FTPassElement::simple_action(Packet* p) {
    return p;
}

Packet* FTPassElement::pull(int port) {
    LOG("------------------------------");
    LOG("Begin FTPassElement");

    //TODO: change port back to zero
    LOG("End FTPassElement");
    LOG("------------------------------");

    return input(port).pull();


}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTPassElement)