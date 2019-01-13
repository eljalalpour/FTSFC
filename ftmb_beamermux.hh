#pragma once

#include <click/config.h>
#include <click/element.hh>
#include <click/ipaddress.hh>

#include "fivetuple.hh"
#include "statetrack.hh"
#include "dipmap.hh"

#include "ftmb_shared_state_beamer.hh"

CLICK_DECLS

#define MAC_HEAD_SIZE   14
#define DEFAULT_CRC     0
#define UDP_HEAD_OFFSET_AFTER_MAC_HEAD 1

/// FTMBBeamerMux is adapted version of Beamer load balancer https://github.com/Beamer-LB/beamer-click
/// The differences are:
///     1- FTMBBeamerMux is not fault tolerant. The original Beamer uses ZooKeeper for fault tolerance
///     2- FTMBBeamerMux handles only UDP packets, while Beamer handles TCP and UDP
///     3- FTMBBeamerMux modifies the IP header or the original packet, while Beamer encapsulate the original packet inside
/// another IP or GRE packet.
class FTMBBeamerMux: public Element {
public:
    FTMBBeamerMux();

    ~FTMBBeamerMux();

    const char *class_name() const { return "FTMBBeamerMux"; }

    const char *port_count() const { return "1/1"; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);

private:
    size_t _queue;
    FTMBSharedStateBeamer* _shared_state;
    String _shared_state_element_name;

    inline void _init_shared_state();

    //IPAddress vip;
    Beamer::DIPHistoryMap bucketMap;

    struct MuxState: public ClickityClack::State<ClickityClack::FiveTuple>
    {
        uint32_t dip;

        MuxState();

        MuxState(ClickityClack::FiveTuple tuple, uint32_t dip)
                : ClickityClack::State<ClickityClack::FiveTuple>(tuple), dip(dip) {}
    };
    ClickityClack::StateTrack<MuxState> **states;
};


CLICK_ENDDECLS