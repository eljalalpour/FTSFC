#ifndef CLICK_DIST_COUNTER_HH
#define CLICK_DIST_COUNTER_HH

#include <click/element.hh>
#include <map>
#include <iostream>
CLICK_DECLS


class distCounter : public Element { public:
    distCounter() CLICK_COLD;
    ~distCounter() CLICK_COLD;

    const char *class_name() const  { return "distCounter"; }
    const char *port_count() const  { return PORTS_1_1; }

    //int configure(Vector<String> &, ErrorHandler *) CLICK_COLD;
    //int initialize(ErrorHandler *) CLICK_COLD;

    Packet *simple_action(Packet *);
private:
    std::map<int, uint64_t> counter;
};


CLICK_ENDDECLS
#endif
