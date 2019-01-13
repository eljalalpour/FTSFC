/*
 * FTMBAddrRewriter.{cc,hh} -- rewrites packet source and destination
 * Eddie Kohler
 *
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2009-2010 Meraki, Inc.
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
#include "ftmb_addrrewriter.hh"
#include <click/args.hh>
#include <click/straccum.hh>
#include <click/error.hh>
#include <clicknet/tcp.h>
#include <clicknet/udp.h>
#include <click/router.hh>
CLICK_DECLS

void
FTMBAddrRewriter::AddrFlow::apply(WritablePacket *p, bool direction,
                                  unsigned annos)
{
    assert(p->has_network_header());
    click_ip *iph = p->ip_header();

    // IP header
    const IPFlowID &revflow = _e[!direction].flowid();
    if (!direction)
        iph->ip_src = revflow.daddr();
    else {
        iph->ip_dst = revflow.saddr();
        if (annos & 1)
            p->set_dst_ip_anno(revflow.saddr());
        if (annos & 2)
            p->set_anno_u8(annos >> 2, _reply_anno);
    }
    update_csum(&iph->ip_sum, direction, _ip_csum_delta);

    // UDP/TCP header
    if (!IP_FIRSTFRAG(iph))
        /* do nothing */;
    else if (iph->ip_p == IP_PROTO_TCP && p->transport_length() >= 18) {
        click_tcp *tcph = p->tcp_header();
        update_csum(&tcph->th_sum, direction, _udp_csum_delta);
    } else if (iph->ip_p == IP_PROTO_UDP && p->transport_length() >= 8) {
        click_udp *udph = p->udp_header();
        if (udph->uh_sum)	// 0 checksum is no checksum
            update_csum(&udph->uh_sum, direction, _udp_csum_delta);
    }
}

void
FTMBAddrRewriter::AddrFlow::unparse(StringAccum &sa, bool direction,
                                    click_jiffies_t now) const
{
    sa << _e[direction].flowid().saddr() << " => "
       << _e[!direction].flowid().daddr();
    unparse_ports(sa, direction, now);
}

FTMBAddrRewriter::FTMBAddrRewriter()
{
}

FTMBAddrRewriter::~FTMBAddrRewriter()
{
}

void *
FTMBAddrRewriter::cast(const char *n)
{
    if (strcmp(n, "RewriterBase") == 0)
        return (RewriterBase *)this;
    else if (strcmp(n, "FTMBAddrRewriter") == 0)
        return (FTMBAddrRewriter *)this;
    else
        return 0;
}

void FTMBAddrRewriter::_init_shared() {
    Router *r = this->router();
    _shared_locks = (SharedLocks *)(r->find(_shared_locks_element_name));
    _shared_state = (FTMBSharedStateAddrRewriter *)(r->find(_shared_state_element_name));

    _shared_state->register_addr_rewriter(_queue, &last_lock_index);
}

int
FTMBAddrRewriter::configure(Vector<String> &conf, ErrorHandler *errh)
{
    bool has_reply_anno = false;
    int reply_anno;
    _timeouts[0] = 60 * 120;	// 2 hours

    if (Args(this, errh).bind(conf)
                .read("REPLY_ANNO", has_reply_anno, AnnoArg(1), reply_anno)
                .read("SHARED_LOCKS", _shared_locks_element_name)
                .read("SHARED_STATE", _shared_state_element_name)
                .read("QUEUE", _queue)
                .consume() < 0)
        return -1;

    _annos = 1 + (has_reply_anno ? 2 + (reply_anno << 2) : 0);

    _init_shared();

    return RewriterBase::configure(conf, errh);
}

RewriterEntry *
FTMBAddrRewriter::get_entry(int, const IPFlowID &xflowid, int input)
{
    IPFlowID flowid(xflowid.saddr(), 0, IPAddress(), 0);
    RewriterEntry *m = _map.get(flowid);
    if (!m) {
        IPFlowID rflowid(IPAddress(), 0, xflowid.daddr(), 0);
        m = _map.get(rflowid);
    }
    if (!m && (unsigned) input < (unsigned) _input_specs.size()) {
        RewriterInput &is = _input_specs[input];
        IPFlowID rewritten_flowid = IPFlowID::uninitialized_t();
        if (is.rewrite_flowid(flowid, rewritten_flowid, 0) == rw_addmap)
            m = add_flow(0, flowid, rewritten_flowid, input);
    }
    return m;
}

RewriterEntry *
FTMBAddrRewriter::add_flow(int, const IPFlowID &flowid,
                         const IPFlowID &rewritten_flowid, int input)
{
    void *data;
    if (rewritten_flowid.sport()
        || rewritten_flowid.dport()
        || rewritten_flowid.daddr()
        || !(data = _allocator.allocate()))
        return 0;

    AddrFlow *flow = new(data) AddrFlow
            (&_input_specs[input], flowid, rewritten_flowid,
             !!_timeouts[1], click_jiffies() + relevant_timeout(_timeouts));

    return store_flow(flow, input, _map);
}

void
FTMBAddrRewriter::push(int port, Packet *p_in)
{
    WritablePacket *p = p_in->uniqueify();
    click_ip *iph = p->ip_header();

    IPFlowID flowid(iph->ip_src, 0, IPAddress(), 0);

    /// Read shared state
    RewriterEntry *m;
    Locker* locker;

    {/// Critical section
        /// A - LOCK
        /// lock the corresponding reader lock
        last_lock_index = _map.bucket(flowid);
        locker = _shared_state->preprocess(last_lock_index, _queue);

        m = _map.get(flowid);

        if (!m) {
            /// A - UNLOCK
            /// unlock the corresponding reader lock
            UNLOCK(locker);

            IPFlowID rflowid = IPFlowID(IPAddress(), 0, iph->ip_dst, 0);

            /// B - LOCK
            /// lock the corresponding reader lock
            last_lock_index = _map.bucket(rflowid);
            locker = _shared_state->preprocess(last_lock_index, _queue);

            /// Read shared state
            m = _map.get(rflowid);
        }

        if (!m) {            // create new mapping
            /// B - UNLOCK
            /// unlock the corresponding reader lock
            UNLOCK(locker);

            RewriterInput &is = _input_specs.unchecked_at(port);
            IPFlowID rewritten_flowid = IPFlowID::uninitialized_t();
            int result = is.rewrite_flowid(flowid, rewritten_flowid, p);
            if (result == rw_addmap) { /// Write shared state

                /// C - LOCK
                /// lock the corresponding lock
                last_lock_index = _map.bucket(rewritten_flowid);
                locker = _shared_state->preprocess(last_lock_index, _queue);

                m = FTMBAddrRewriter::add_flow(0, flowid, rewritten_flowid, port);
//                _shared_locks->extend_size(_map.bucket_count());

            }//if
            if (!m) {
                /// C - UNLOCK
                _shared_state->postprocess(_queue, locker, 0, &output(result));
                checked_output_push(result, p);
                return;
            }//if
            else if (_annos & 2) {
                m->flow()->set_reply_anno(p->anno_u8(_annos >> 2));
            }//else if
        }//if

        /// Read shared state
        AddrFlow *mf = static_cast<AddrFlow *>(m->flow());

        /// Read shared state
        mf->apply(p, m->direction(), _annos);

        /// Write shared state
        mf->change_expiry_by_timeout(_heap, click_jiffies(), _timeouts);

        /// A, B, C - WRITER UNLOCK
        /// unlock the corresponding writer lock
        _shared_state->postprocess(_queue, locker, p, &output(m->output()));
    }
//    output(m->output()).push(p);
}


String
FTMBAddrRewriter::dump_mappings_handler(Element *e, void *)
{
    FTMBAddrRewriter *rw = (FTMBAddrRewriter *)e;
    StringAccum sa;
    click_jiffies_t now = click_jiffies();
    for (Map::iterator iter = rw->_map.begin(); iter.live(); iter++) {
        AddrFlow *f = static_cast<AddrFlow *>(iter->flow());
        f->unparse(sa, iter->direction(), now);
        sa << '\n';
    }
    return sa.take_string();
}

void
FTMBAddrRewriter::add_handlers()
{
    add_read_handler("table", dump_mappings_handler);
    add_read_handler("mappings", dump_mappings_handler, 0, Handler::h_deprecated);
    add_rewriter_handlers(true);
}

ELEMENT_REQUIRES(SharedLocks)
ELEMENT_REQUIRES(RewriterBase)
EXPORT_ELEMENT(FTMBAddrRewriter)
CLICK_ENDDECLS