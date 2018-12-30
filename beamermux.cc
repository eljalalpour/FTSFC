#include "beamermux.hh"
#include <click/args.hh>
#include <clicknet/tcp.h>
#include <clicknet/udp.h>
#include <click/error.hh>

CLICK_DECLS

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

static const int RESERVED_PORT_COUNT = 1024;

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

    vip = 0;
    bucketMap.init(ringSize);

    idMap.init(0x10000);

    states = new StateTrack<MuxState>*[click_max_cpu_ids()]; assert(states);
    for (int i = 0; i < click_max_cpu_ids(); i++)
    {
        states[i] = new StateTrack<MuxState>(4 * 60 * CLICK_HZ, maxStates / click_max_cpu_ids()); assert(states[i]);
    }

    return 0;
}

Packet *BeamerMux::handleUDP(Packet *p) {
    uint32_t hash = beamerHash(p->ip_header(), p->udp_header());
    uint32_t dip = bucketMap.get(hash).current;

    return ipipEncapper.encapsulate(p, vip.addr(), dip);
}

Packet *BeamerMux::simple_action(Packet *p)
{
    uint8_t proto = p->ip_header()->ip_p;
    unsigned int cpuID = click_current_cpu_id();
    click_jiffies_t now = click_jiffies();

    switch (proto)
    {
        case IPPROTO_UDP:
            return handleUDP(p);

        default:
            return p;
    }

    p->kill();
    return NULL;
}

enum
{
    /* write */
            H_ASSIGN,

    /* read */
            H_GEN,
};

CLICK_ENDDECLS

EXPORT_ELEMENT(BeamerMux)
