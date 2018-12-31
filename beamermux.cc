#include "beamermux.hh"
#include <click/args.hh>
#include <clicknet/tcp.h>
#include <clicknet/udp.h>
#include <click/error.hh>
#include <click/packet_anno.hh>
#include <clicknet/ip.h>
#include "checksumfixup.hh"

CLICK_DECLS

using namespace ClickityClack;

#define CLICK_BEAMER_HASHFN_BOB 0
#define CLICK_BEAMER_HASHFN_CRC 1

#ifndef CLICK_BEAMER_HASHFN
#define CLICK_BEAMER_HASHFN CLICK_BEAMER_HASHFN_CRC
#endif

#define CLICK_BEAMER_STATEFUL_DAISY 0

static inline uint32_t beamerHash(const click_ip *ipHeader, const click_tcp *tcpHeader)
{
#if CLICK_BEAMER_HASHFN == CLICK_BEAMER_HASHFN_BOB
    return freeBSDBob(ipHeader->ip_src.s_addr, tcpHeader->th_sport, tcpHeader->th_dport);
#elif CLICK_BEAMER_HASHFN == CLICK_BEAMER_HASHFN_CRC
    struct HashTouple touple = { ipHeader->ip_src.s_addr, tcpHeader->th_sport };
    return p4_crc32_6((char *)&touple);
#else
#error Invalid CLICK_BEAMER_HASHFN
#endif
}

static inline uint32_t beamerHash(const click_ip *ipHeader, const click_udp *udpHeader)
{
#if CLICK_BEAMER_HASHFN == CLICK_BEAMER_HASHFN_BOB
    return freeBSDBob(ipHeader->ip_src.s_addr, udpHeader->uh_sport, udpHeader->uh_dport);
#elif CLICK_BEAMER_HASHFN == CLICK_BEAMER_HASHFN_CRC
    struct HashTouple touple = { ipHeader->ip_src.s_addr, udpHeader->uh_sport };
    return p4_crc32_6((char *)&touple);
#else
#error Invalid CLICK_BEAMER_HASHFN
#endif
}

BeamerMux::BeamerMux() {}

BeamerMux::~BeamerMux() {}

int BeamerMux::configure(Vector<String> &conf, ErrorHandler *errh) {
    int ringSize = 1;
    int maxStates = -1;

    if (Args(conf, this, errh)
                .read("RING_SIZE",  BoundedIntArg(0, (int)0x800000), ringSize)
                .read("MAX_STATES", IntArg(),                        maxStates)
                .complete() < 0) {
        return -1;
    }

    if (maxStates <= 0)
        return errh->error("Bad MAX_STATES");

    //vip = 0;
    bucketMap.init(ringSize);

    states = new StateTrack<MuxState>*[click_max_cpu_ids()]; assert(states);
    for (int i = 0; i < click_max_cpu_ids(); i++)
    {
        states[i] = new StateTrack<MuxState>(4 * 60 * CLICK_HZ, maxStates / click_max_cpu_ids()); assert(states[i]);
    }

    return 0;
}



Packet *BeamerMux::simple_action(Packet *p) {
    // We assume that the packet is always UDP
    uint32_t hash = beamerHash(p->ip_header(), p->udp_header());
    uint32_t dip = bucketMap.get(hash).current;

    // Update the header,
    // part of the code is taken from ipipEcaper.encapsulate function in ClickityClack
    click_ip *ip = reinterpret_cast<click_ip *>(p->data() + MAC_HEAD_SIZE);
    click_udp *udp = reinterpret_cast<click_udp *>(ip + UDP_HEAD_OFFSET_AFTER_MAC_HEAD);

    ip->ip_hl = sizeof(click_ip) >> 2;
    ip->ip_dst.s_addr = dip;
    udp->uh_sum = DEFAULT_CRC;

    ip->ip_sum = checksumFold(
            checksumFixup32(0, ip->ip_src.,
                            checksumFixup32(0, dip,
                                            checksumFixup16(0, ip->ip_len,
                                                            ip->ip_sum))));

    p->set_ip_header(ip, sizeof(click_ip));

    return p;
}

CLICK_ENDDECLS

EXPORT_ELEMENT(BeamerMux)
