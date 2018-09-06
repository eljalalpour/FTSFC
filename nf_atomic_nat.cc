#include "atomic_array.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "nf_atomic_counter.hh"

CLICK_DECLS

NFAtomicNAT::NFAtomicNAT () : _index(DEFAULT_INDEX) { };

NFAtomicNAT::~NFAtomicNAT() { };

Packet *NFAtomicNAT::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin NFAtomicNAT with index %d:", _index);
    Router *r = this->router();

    click_ip *iph = rp->ip_header();



    AtomicArray *afc = (AtomicArray *)(r->find("counters"));
    ++afc->counters[_index];

    LOG("End NFAtomicNAT %d:", _index);
    LOG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NFAtomicNAT)
