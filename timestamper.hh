// -*- c-basic-offset: 4 -*-
#ifndef CLICK_TIMESTAMPER_HH
#define CLICK_TIMESTAMPER_HH
#include <click/element.hh>
CLICK_DECLS

/*
=c

TimestampAccum()

=s timestamps

collects differences in timestamps

=d

For each passing packet, measures the elapsed time since the packet's
timestamp. Keeps track of the total elapsed time accumulated over all packets.

=h count read-only
Returns the number of packets that have passed.

=h time read-only
Returns the accumulated timestamp difference for all passing packets.

=h average_time read-only
Returns the average timestamp difference over all passing packets.

=h reset_counts write-only
Resets C<count> and C<time> counters to zero when written.

=a SetCycleCount, RoundTripCycleCount, SetPerfCount, PerfCountAccum */

class Timestamper : public Element { public:

    Timestamper() CLICK_COLD;
    ~Timestamper() CLICK_COLD;

    const char *class_name() const	{ return "Timestamper"; }
    const char *port_count() const	{ return "2/2"; }

    int initialize(ErrorHandler *) CLICK_COLD;
    void add_handlers() CLICK_COLD;
    void push(int port, Packet *p);
//    Packet *simple_action(Packet *);

  private:

    double _usec_accum;
    double _usec_base;
    uint64_t _count;

    static String read_handler(Element *, void *) CLICK_COLD;
    static int reset_handler(const String &, Element *, void *, ErrorHandler *);

};

CLICK_ENDDECLS
#endif
