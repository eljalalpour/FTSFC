/*
 * burner.{cc,hh} -- elements hault there and burn cpu to sumulate
 * the action of processing packets.
 *
 * Copyright (c) 2017 University of Waterloo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

#include <click/config.h>
#include "burner.hh"

CLICK_DECLS

Burner::Burner() : _cycle(10)
{
}

Burner::~Burner()
{
}

Packet*
Burner::simple_action(Packet *p)
{
    unsigned int burn = 1;
    //unsigned int burn = rand() % 50 + 1;
    //for (int i = 0; i < _cycle; i++) {
    //    burn = (burn + 49) % 33;
    //}
    while (1) {
        burn++;
        if (burn > _cycle)
            break;
    }
    return p;
}

int
Burner::configure(Vector<String> &conf, ErrorHandler *errh)
{
    if (Args(conf, this, errh).read_or_set("CYCLE", _cycle, 10).complete() < 0)
        return -1;
     
    printf("burning cycle is %d!\n", _cycle);

    return 0;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(Burner)
