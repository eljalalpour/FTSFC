#include <click/config.h>
#include <click/args.hh>
#include "FTRandomDropElement.hh"
CLICK_DECLS

FTRandomDropElement::FTRandomDropElement(): _all(0), _dropped(0) { }

FTRandomDropElement::~FTRandomDropElement() { }

void FTRandomDropElement::push(int, Packet *p) {
    ++_all;

    DEBUG("------------------------------");
    DEBUG("Begin FTRandomDropElement");

    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    if (r <= _dropChance) {
        ++_dropped;
        p->kill();

        DEBUG("All: %d, Dropped: %d", _all, _dropped);
        return;
    }//if

    DEBUG("All: %d, Dropped: %d", _all, _dropped);
    DEBUG("End FTRandomDropElement");
    DEBUG("------------------------------");

    output(0).push(p);
}

int FTRandomDropElement::configure(Vector<String> &conf, ErrorHandler *errh) {
    DoubleArg parser;
    for (int i = 0; i < conf.size(); ++i) {
        parser.parse(conf[i], _dropChance);
        if (_dropChance > 1.0 || _dropChance < 0.0) {
            DEBUG("The chance of dropping a packet cannot be bigger than a 1.0, or less than 0.0!");
            return -1;
        }//if
    }//for

    return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTRandomDropElement)