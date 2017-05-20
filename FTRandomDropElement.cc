#include <click/config.h>
#include <click/args.hh>
#include "FTRandomDropElement.hh"

CLICK_DECLS

FTRandomDropElement::FTRandomDropElement() { }

FTRandomDropElement::~FTRandomDropElement() { }

void FTRandomDropElement::push(int, Packet *p) {
    click_chatter("------------------------------");
    click_chatter("Begin FTRandomDropElement");

    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    if (r <= _dropChance) {
        click_chatter("Dropping a packet!");
        p->kill();
        return;
    }//if

    click_chatter("Passing a packet!");
    output(0).push(p);

    click_chatter("------------------------------");
}

int FTRandomDropElement::configure(Vector<String> &conf, ErrorHandler *errh) {
    DoubleArg parser;
    for (int i = 0; i < conf.size(); ++i) {
        parser.parse(conf[i], _dropChance);
        if (_dropChance > 1.0 || _dropChance < 0.0) {
            click_chatter("The chance of dropping a packet cannot be bigger than a 1.0, or less than 0.0!");
            return -1;
        }//if
        click_chatter("The chance of dropping a packet: %f", _dropChance);
    }//for

    return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTRandomDropElement)