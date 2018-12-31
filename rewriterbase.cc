// -*- mode: c++; c-basic-offset: 4 -*-
/*
 * RewriterBase.{cc,hh} -- rewrites packet source and destination
 * Eddie Kohler
 * original versions by Eddie Kohler and Max Poletto
 *
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2001 International Computer Science Institute
 * Copyright (c) 2008-2010 Meraki, Inc.
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
#include "rewriterbase.hh"
#include "rwpatterns.hh"
#include "rwmapping.hh"
#include "rwpattern.hh"
#include <clicknet/ip.h>
#include <clicknet/tcp.h>
#include <clicknet/udp.h>
#include <click/llrpc.h>
#include <click/args.hh>
#include <click/straccum.hh>
#include <click/error.hh>
#include <click/algorithm.hh>
#include <click/heap.hh>

#ifdef CLICK_LINUXMODULE
#include <click/cxxprotect.h>
CLICK_CXX_PROTECT
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0)
# include <asm/softirq.h>
#endif
#include <net/sock.h>
CLICK_CXX_UNPROTECT
#include <click/cxxunprotect.h>
#endif

CLICK_DECLS

//
// Mapper
//

void
Mapper::notify_rewriter(RewriterBase *, RewriterInput *, ErrorHandler *)
{
}

int
Mapper::rewrite_flowid(RewriterInput *, const IPFlowID &, IPFlowID &,
                         Packet *, int)
{
    return RewriterBase::rw_drop;
}

//
// RewriterBase
//

RewriterBase::RewriterBase()
        : _map(0), _heap(new RewriterHeap), _gc_timer(gc_timer_hook, this)
{
    _timeouts[0] = default_timeout;
    _timeouts[1] = default_guarantee;
    _gc_interval_sec = default_gc_interval;
}

RewriterBase::~RewriterBase()
{
    if (_heap)
        _heap->unuse();
}


int
RewriterBase::parse_input_spec(const String &line, RewriterInput &is,
                                 int input_number, ErrorHandler *errh)
{
    PrefixErrorHandler cerrh(errh, "input spec " + String(input_number) + ": ");
    String word, rest;
    if (!cp_word(line, &word, &rest))
        return cerrh.error("empty argument");
    cp_eat_space(rest);

    is.kind = RewriterInput::i_drop;
    is.owner = this;
    is.owner_input = input_number;
    is.reply_element = this;

    if (word == "pass" || word == "passthrough" || word == "nochange") {
        int32_t outnum = 0;
        if (rest && !IntArg().parse(rest, outnum))
            return cerrh.error("syntax error, expected %<nochange [OUTPUT]%>");
        else if ((unsigned) outnum >= (unsigned) noutputs())
            return cerrh.error("output port out of range");
        is.kind = RewriterInput::i_nochange;
        is.foutput = outnum;

    } else if (word == "keep") {
        Vector<String> words;
        cp_spacevec(rest, words);
        if (!RewriterPattern::parse_ports(words, &is, this, &cerrh))
            return -1;
        if ((unsigned) is.foutput >= (unsigned) noutputs()
            || (unsigned) is.routput >= (unsigned) is.reply_element->noutputs())
            return cerrh.error("output port out of range");
        is.kind = RewriterInput::i_keep;

    } else if (word == "drop" || word == "discard") {
        if (rest)
            return cerrh.error("syntax error, expected %<%s%>", word.c_str());

    } else if (word == "pattern" || word == "xpattern") {
        if (!RewriterPattern::parse_with_ports(rest, &is, this, &cerrh))
            return -1;
        if ((unsigned) is.foutput >= (unsigned) noutputs()
            || (unsigned) is.routput >= (unsigned) is.reply_element->noutputs())
            return cerrh.error("output port out of range");
        is.u.pattern->use();
        is.kind = RewriterInput::i_pattern;

    } else if (Element *e = cp_element(word, this, 0)) {
        Mapper *mapper = (Mapper *)e->cast("Mapper");
        if (rest)
            return cerrh.error("syntax error, expected element name");
        else if (!mapper)
            return cerrh.error("element is not an Mapper");
        else {
            is.kind = RewriterInput::i_mapper;
            is.u.mapper = mapper;
        }

    } else
        return cerrh.error("unknown specification");

    return 0;
}

int
RewriterBase::configure(Vector<String> &conf, ErrorHandler *errh)
{
    String capacity_word;

    if (Args(this, errh).bind(conf)
                .read("CAPACITY", AnyArg(), capacity_word)
                .read("MAPPING_CAPACITY", AnyArg(), capacity_word)
                .read("TIMEOUT", SecondsArg(), _timeouts[0])
                .read("GUARANTEE", SecondsArg(), _timeouts[1])
                .read("REAP_INTERVAL", SecondsArg(), _gc_interval_sec)
                .read("REAP_TIME", Args::deprecated, SecondsArg(), _gc_interval_sec)
                .consume() < 0)
        return -1;

    if (capacity_word) {
        Element *e;
        RewriterBase *rwb;
        if (IntArg().parse(capacity_word, _heap->_capacity))
            /* OK */;
        else if ((e = cp_element(capacity_word, this))
                 && (rwb = (RewriterBase *) e->cast("RewriterBase"))) {
            rwb->_heap->use();
            _heap->unuse();
            _heap = rwb->_heap;
        } else
            return errh->error("bad MAPPING_CAPACITY");
    }

    if (conf.size() != ninputs())
        return errh->error("need %d arguments, one per input port", ninputs());

    _timeouts[0] *= CLICK_HZ;	// _timeouts is measured in jiffies
    _timeouts[1] *= CLICK_HZ;

    for (int i = 0; i < conf.size(); ++i) {
        RewriterInput is;
        if (parse_input_spec(conf[i], is, i, errh) >= 0)
            _input_specs.push_back(is);
    }

    return _input_specs.size() == ninputs() ? 0 : -1;
}

int
RewriterBase::initialize(ErrorHandler *errh)
{
    for (int i = 0; i < _input_specs.size(); ++i) {
        PrefixErrorHandler cerrh(errh, "input spec " + String(i) + ": ");
        if (_input_specs[i].reply_element->_heap != _heap)
            cerrh.error("reply element %<%s%> must share this MAPPING_CAPACITY", i, _input_specs[i].reply_element->name().c_str());
        if (_input_specs[i].kind == RewriterInput::i_mapper)
            _input_specs[i].u.mapper->notify_rewriter(this, &_input_specs[i], &cerrh);
    }
    _gc_timer.initialize(this);
    if (_gc_interval_sec)
        _gc_timer.schedule_after_sec(_gc_interval_sec);
    return errh->nerrors() ? -1 : 0;
}

void
RewriterBase::cleanup(CleanupStage)
{
    shrink_heap(true);
    for (int i = 0; i < _input_specs.size(); ++i)
        if (_input_specs[i].kind == RewriterInput::i_pattern)
            _input_specs[i].u.pattern->unuse();
    _input_specs.clear();
}

RewriterEntry *
RewriterBase::get_entry(int ip_p, const IPFlowID &flowid, int input)
{
    RewriterEntry *m = _map.get(flowid);
    if (m && ip_p && m->flow()->ip_p() && m->flow()->ip_p() != ip_p)
        return 0;
    if (!m && (unsigned) input < (unsigned) _input_specs.size()) {
        RewriterInput &is = _input_specs[input];
        IPFlowID rewritten_flowid = IPFlowID::uninitialized_t();
        if (is.rewrite_flowid(flowid, rewritten_flowid, 0) == rw_addmap)
            m = add_flow(ip_p, flowid, rewritten_flowid, input);
    }
    return m;
}

RewriterEntry *
RewriterBase::store_flow(RewriterFlow *flow, int input,
                           Map &map, Map *reply_map_ptr)
{
    RewriterBase *reply_element = _input_specs[input].reply_element;
    if ((unsigned) flow->entry(false).output() >= (unsigned) noutputs()
        || (unsigned) flow->entry(true).output() >= (unsigned) reply_element->noutputs()) {
        flow->owner()->owner->destroy_flow(flow);
        return 0;
    }

    RewriterEntry *old = map.set(&flow->entry(false));
    assert(!old);

    if (!reply_map_ptr)
        reply_map_ptr = &reply_element->_map;
    old = reply_map_ptr->set(&flow->entry(true));
    if (unlikely(old)) {		// Assume every map has the same heap.
        if (likely(old->flow() != flow))
            old->flow()->destroy(_heap);
    }

    Vector<RewriterFlow *> &myheap = _heap->_heaps[flow->guaranteed()];
    myheap.push_back(flow);
    push_heap(myheap.begin(), myheap.end(),
              RewriterFlow::heap_less(), RewriterFlow::heap_place());
    ++_input_specs[input].count;

    if (unlikely(_heap->size() > _heap->capacity())) {
        // This may destroy the newly added mapping, if it has the lowest
        // expiration time.  How can we tell?  If (1) flows are added to the
        // heap one at a time, so the heap was formerly no bigger than the
        // capacity, and (2) 'flow' expires in the future, then we will only
        // destroy 'flow' if it's the top of the heap.
        click_jiffies_t now_j = click_jiffies();
        assert(click_jiffies_less(now_j, flow->expiry())
               && _heap->size() == _heap->capacity() + 1);
        if (shrink_heap_for_new_flow(flow, now_j)) {
            ++_input_specs[input].failures;
            return 0;
        }
    }

    if (map.unbalanced())
        map.rehash(map.bucket_count() + 1);
    if (reply_map_ptr != &map && reply_map_ptr->unbalanced())
        reply_map_ptr->rehash(reply_map_ptr->bucket_count() + 1);
    return &flow->entry(false);
}

void
RewriterBase::shift_heap_best_effort(click_jiffies_t now_j)
{
    // Shift flows with expired guarantees to the best-effort heap.
    Vector<RewriterFlow *> &guaranteed_heap = _heap->_heaps[1];
    while (guaranteed_heap.size() && guaranteed_heap[0]->expired(now_j)) {
        RewriterFlow *mf = guaranteed_heap[0];
        click_jiffies_t new_expiry = mf->owner()->owner->best_effort_expiry(mf);
        mf->change_expiry(_heap, false, new_expiry);
    }
}

bool
RewriterBase::shrink_heap_for_new_flow(RewriterFlow *flow,
                                         click_jiffies_t now_j)
{
    shift_heap_best_effort(now_j);
    // At this point, all flows in the guarantee heap expire in the future.
    // So remove the next-to-expire best-effort flow, unless there are none.
    // In that case we always remove the current flow to honor previous
    // guarantees (= admission control).
    RewriterFlow *deadf;
    if (_heap->_heaps[0].empty()) {
        assert(flow->guaranteed());
        deadf = flow;
    } else
        deadf = _heap->_heaps[0][0];
    deadf->destroy(_heap);
    return deadf == flow;
}

void
RewriterBase::shrink_heap(bool clear_all)
{
    click_jiffies_t now_j = click_jiffies();
    shift_heap_best_effort(now_j);
    Vector<RewriterFlow *> &best_effort_heap = _heap->_heaps[0];
    while (best_effort_heap.size() && best_effort_heap[0]->expired(now_j))
        best_effort_heap[0]->destroy(_heap);

    int32_t capacity = clear_all ? 0 : _heap->_capacity;
    while (_heap->size() > capacity) {
        RewriterFlow *deadf = _heap->_heaps[_heap->_heaps[0].empty()][0];
        deadf->destroy(_heap);
    }
}

void
RewriterBase::gc_timer_hook(Timer *t, void *user_data)
{
    RewriterBase *rw = static_cast<RewriterBase *>(user_data);
    rw->shrink_heap(false);
    if (rw->_gc_interval_sec)
        t->reschedule_after_sec(rw->_gc_interval_sec);
}

String
RewriterBase::read_handler(Element *e, void *user_data)
{
    RewriterBase *rw = static_cast<RewriterBase *>(e);
    intptr_t what = reinterpret_cast<intptr_t>(user_data);
    StringAccum sa;

    switch (what) {
        case h_nmappings: {
            uint32_t count = 0;
            for (int i = 0; i < rw->_input_specs.size(); ++i)
                count += rw->_input_specs[i].count;
            sa << count;
            break;
        }
        case h_mapping_failures: {
            uint32_t count = 0;
            for (int i = 0; i < rw->_input_specs.size(); ++i)
                count += rw->_input_specs[i].failures;
            sa << count;
            break;
        }
        case h_size:
            sa << rw->_heap->size();
            break;
        case h_capacity:
            sa << rw->_heap->_capacity;
            break;
        default:
            for (int i = 0; i < rw->_input_specs.size(); ++i) {
                if (what != h_patterns && what != i)
                    continue;
                switch (rw->_input_specs[i].kind) {
                    case RewriterInput::i_drop:
                        sa << "<drop>";
                        break;
                    case RewriterInput::i_nochange:
                        sa << "<nochange>";
                        break;
                    case RewriterInput::i_keep:
                        sa << "<keep>";
                        break;
                    case RewriterInput::i_pattern:
                        sa << rw->_input_specs[i].u.pattern->unparse();
                        break;
                    case RewriterInput::i_mapper:
                        sa << "<mapper>";
                        break;
                }
                if (rw->_input_specs[i].count)
                    sa << " [" << rw->_input_specs[i].count << ']';
                sa << '\n';
            }
            break;
    }
    return sa.take_string();
}

int
RewriterBase::write_handler(const String &str, Element *e, void *user_data, ErrorHandler *errh)
{
    RewriterBase *rw = static_cast<RewriterBase *>(e);
    intptr_t what = reinterpret_cast<intptr_t>(user_data);
    if (what == h_capacity) {
        if (Args(e, errh).push_back_words(str)
                                  .read_mp("CAPACITY", rw->_heap->_capacity)
                                  .complete() < 0)
        return -1;
        rw->shrink_heap(false);
        return 0;
    } else if (what == h_clear) {
        rw->shrink_heap(true);
        return 0;
    } else
        return -1;
}

int
RewriterBase::pattern_write_handler(const String &str, Element *e, void *user_data, ErrorHandler *errh)
{
    RewriterBase *rw = static_cast<RewriterBase *>(e);
    intptr_t what = reinterpret_cast<intptr_t>(user_data);
    RewriterInput is;
    int r = rw->parse_input_spec(str, is, what, errh);
    if (r >= 0) {
        RewriterInput *spec = &rw->_input_specs[what];

        // remove all existing flows created by this input
        for (int which_heap = 0; which_heap < 2; ++which_heap) {
            Vector<RewriterFlow *> &myheap = rw->_heap->_heaps[which_heap];
            for (int i = myheap.size() - 1; i >= 0; --i)
                if (myheap[i]->owner() == spec) {
                    myheap[i]->destroy(rw->_heap);
                    if (i < myheap.size())
                        ++i;
                }
        }

        // change pattern
        if (spec->kind == RewriterInput::i_pattern)
            spec->u.pattern->unuse();
        *spec = is;
    }
    return 0;
}

void
RewriterBase::add_rewriter_handlers(bool writable_patterns)
{
    add_read_handler("table_size", read_handler, h_nmappings);
    add_read_handler("nmappings", read_handler, h_nmappings, Handler::h_deprecated);
    add_read_handler("mapping_failures", read_handler, h_mapping_failures);
    add_read_handler("patterns", read_handler, h_patterns);
    add_read_handler("size", read_handler, h_size);
    add_read_handler("capacity", read_handler, h_capacity);
    add_write_handler("capacity", write_handler, h_capacity);
    add_write_handler("clear", write_handler, h_clear);
    for (int i = 0; i < ninputs(); ++i) {
        String name = "pattern" + String(i);
        add_read_handler(name, read_handler, i);
        if (writable_patterns)
            add_write_handler(name, pattern_write_handler, i);
    }
}

int
RewriterBase::llrpc(unsigned command, void *data)
{
    if (command == CLICK_LLRPC_IPREWRITER_MAP_TCP) {
        // Data	: unsigned saddr, daddr; unsigned short sport, dport
        // Incoming : the flow ID
        // Outgoing : If there is a mapping for that flow ID, then stores the
        //	      mapping into 'data' and returns zero. Otherwise, returns
        //	      -EAGAIN.

        IPFlowID *val = reinterpret_cast<IPFlowID *>(data);
        RewriterEntry *m = get_entry(IP_PROTO_TCP, *val, -1);
        if (!m)
            return -EAGAIN;
        *val = m->rewritten_flowid();
        return 0;

    } else if (command == CLICK_LLRPC_IPREWRITER_MAP_UDP) {
        // Data	: unsigned saddr, daddr; unsigned short sport, dport
        // Incoming : the flow ID
        // Outgoing : If there is a mapping for that flow ID, then stores the
        //	      mapping into 'data' and returns zero. Otherwise, returns
        //	      -EAGAIN.

        IPFlowID *val = reinterpret_cast<IPFlowID *>(data);
        RewriterEntry *m = get_entry(IP_PROTO_UDP, *val, -1);
        if (!m)
            return -EAGAIN;
        *val = m->rewritten_flowid();
        return 0;

    } else
        return Element::llrpc(command, data);
}

ELEMENT_REQUIRES(RewriterMapping RewriterPattern)
ELEMENT_PROVIDES(RewriterBase)
CLICK_ENDDECLS