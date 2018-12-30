#ifndef CLICK_CLICKITYCLACK_FIVETUPLE_HH
#define CLICK_CLICKITYCLACK_FIVETUPLE_HH

#include <click/config.h>
#include <clicknet/ip.h>
#include <clicknet/tcp.h>
#include <clicknet/udp.h>
#include <clicknet/icmp.h>
#include "freebsdbob.hh"

CLICK_DECLS

namespace ClickityClack
{

    struct FiveTuple
    {
/* constant missing from ClickOS */
#ifdef CLICK_OS
        static const uint8_t IPPROTO_ICMP = 1;
#endif

        uint32_t srcIP;
        uint32_t dstIP;
        uint16_t srcPort;
        uint16_t dstPort;
        uint8_t proto;

        FiveTuple() {}

        FiveTuple(uint32_t srcIP, uint32_t dstIP, uint16_t srcPort, uint16_t dstPort, uint8_t proto)
                : srcIP(srcIP), dstIP(dstIP), srcPort(srcPort), dstPort(dstPort), proto(proto) {}

        FiveTuple(const click_ip *iph, const click_tcp *tcph)
        //: FiveTuple(iph->ip_src.s_addr, iph->ip_dst.s_addr, tcph->th_sport, tcph->th_dport, IPPROTO_TCP) {}
                : srcIP(iph->ip_src.s_addr), dstIP(iph->ip_dst.s_addr), srcPort(tcph->th_sport), dstPort(tcph->th_dport), proto(IPPROTO_TCP) {}

        FiveTuple(const click_ip *iph, const click_udp *udph)
        //: FiveTuple(iph->ip_src.s_addr, iph->ip_dst.s_addr, udph->uh_sport, udph->uh_dport, IPPROTO_UDP) {}
                : srcIP(iph->ip_src.s_addr), dstIP(iph->ip_dst.s_addr), srcPort(udph->uh_sport), dstPort(udph->uh_dport), proto(IPPROTO_UDP) {}

        FiveTuple(const click_ip *iph, const click_icmp_sequenced *icmph)
        //: FiveTuple(iph->ip_src.s_addr, iph->ip_dst.s_addr, icmph->icmp_identifier, icmph->icmp_identifier, IPPROTO_ICMP) {}
                : srcIP(iph->ip_src.s_addr), dstIP(iph->ip_dst.s_addr), srcPort(icmph->icmp_identifier), dstPort(icmph->icmp_identifier), proto(IPPROTO_ICMP) {}

        FiveTuple reverse() const
        {
            return FiveTuple(dstIP, srcIP, dstPort, srcPort, proto);
        }

        uint32_t hashcode() const
        {
            return ClickityClack::freeBSDBob(ClickityClack::freeBSDBob(srcIP, dstIP, srcPort), dstPort, proto);
        }

        bool operator ==(const FiveTuple &other) const
        {
            return srcIP == other.srcIP &&
                   dstIP == other.dstIP &&
                   srcPort == other.srcPort &&
                   dstPort == other.dstPort &&
                   proto == other.proto;
        }
    };

}

CLICK_ENDDECLS


#endif // CLICK_CLICKITYCLACK_FIVETUPLE_HH