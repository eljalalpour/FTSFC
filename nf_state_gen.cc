#include "shared_state.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include <cstring>
#include "nf_state_gen.hh"

CLICK_DECLS

NFStateGen::NFStateGen () { };

NFStateGen::~NFStateGen() { };

void NFStateGen::_init_state() {
    vect1.reserve(_state_size);
    vect2.reserve(_state_size);

    for (int i = 0; i < _state_size; ++i) {
        vect1.emplace_back(1);
        vect2.emplace_back(2);
    }//for
}

int NFStateGen::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("STATE_SIZE", _state_size)
                .complete() < 0)
        return -1;

    DEBUG("NFStateGen state size is %d!\n", _state_size);

    _init_state();

    return 0;
}

Packet *NFStateGen::simple_action(Packet *p) {
    std::memcpy(const_cast<int*>(vect2.data()), vect1.data(), vect2.size()*sizeof(int));

    return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(SharedState)
EXPORT_ELEMENT(NFStateGen)
