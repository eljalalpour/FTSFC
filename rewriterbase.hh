// -*- mode: c++; c-basic-offset: 4 -*-
#ifndef CLICK_REWRITERBASE_HH
#define CLICK_REWRITERBASE_HH
#include <click/timer.hh>
#include "rwmapping.hh"
#include <click/bitvector.hh>
CLICK_DECLS
class Mapper;
class RewriterPattern;

class RewriterInput { public:
    enum {
        i_drop, i_nochange, i_keep, i_pattern, i_mapper
    };
    RewriterBase *owner;
    int owner_input;
    int kind;
    int foutput;
    RewriterBase *reply_element;
    int routput;
    uint32_t count;
    uint32_t failures;
    union {
        RewriterPattern *pattern;
        Mapper *mapper;
    } u;

    RewriterInput()
            : kind(i_drop), foutput(-1), routput(-1), count(0), failures(0) {
        u.pattern = 0;
    }

    enum {
        mapid_default = 0, mapid_iprewriter_udp = 1
    };

    inline int rewrite_flowid(const IPFlowID &flowid,
                              IPFlowID &rewritten_flowid,
                              Packet *p, int mapid = mapid_default);
};

class RewriterHeap { public:

    RewriterHeap()
            : _capacity(0x7FFFFFFF), _use_count(1) {
    }
    ~RewriterHeap() {
        assert(size() == 0);
    }

    void use() {
        ++_use_count;
    }
    void unuse() {
        assert(_use_count > 0);
        if (--_use_count == 0)
            delete this;
    }

    Vector<RewriterFlow *>::size_type size() const {
        return _heaps[0].size() + _heaps[1].size();
    }
    int32_t capacity() const {
        return _capacity;
    }

private:

    enum {
        h_best_effort = 0, h_guarantee = 1
    };
    Vector<RewriterFlow *> _heaps[2];
    int32_t _capacity;
    uint32_t _use_count;

    friend class RewriterBase;
    friend class RewriterFlow;

};

class RewriterBase : public Element { public:

    typedef HashContainer<RewriterEntry> Map;
    enum {
        rw_drop = -1, rw_addmap = -2
    };

    RewriterBase() CLICK_COLD;
    ~RewriterBase() CLICK_COLD;

    enum ConfigurePhase {
        CONFIGURE_PHASE_PATTERNS = CONFIGURE_PHASE_INFO,
        CONFIGURE_PHASE_REWRITER = CONFIGURE_PHASE_DEFAULT,
        CONFIGURE_PHASE_MAPPER = CONFIGURE_PHASE_REWRITER - 1,
        CONFIGURE_PHASE_USER = CONFIGURE_PHASE_REWRITER + 1
    };

    const char *port_count() const	{ return "1-/1-"; }
    const char *processing() const	{ return PUSH; }

    int configure_phase() const		{ return CONFIGURE_PHASE_REWRITER; }
    int configure(Vector<String> &conf, ErrorHandler *errh) CLICK_COLD;
    int initialize(ErrorHandler *errh) CLICK_COLD;
    void add_rewriter_handlers(bool writable_patterns);
    void cleanup(CleanupStage) CLICK_COLD;

    const RewriterHeap *flow_heap() const {
        return _heap;
    }
    RewriterBase *reply_element(int input) const {
        return _input_specs[input].reply_element;
    }
    virtual HashContainer<RewriterEntry> *get_map(int mapid) {
        return likely(mapid == RewriterInput::mapid_default) ? &_map : 0;
    }

    enum {
        get_entry_check = -1, get_entry_reply = -2
    };
    virtual RewriterEntry *get_entry(int ip_p, const IPFlowID &flowid,
                                       int input);
    virtual RewriterEntry *add_flow(int ip_p, const IPFlowID &flowid,
                                      const IPFlowID &rewritten_flowid,
                                      int input) = 0;
    virtual void destroy_flow(RewriterFlow *flow) = 0;
    virtual click_jiffies_t best_effort_expiry(const RewriterFlow *flow) {
        return flow->expiry() + _timeouts[0] - _timeouts[1];
    }

    int llrpc(unsigned command, void *data);

protected:

    Map _map;

    Vector<RewriterInput> _input_specs;

    RewriterHeap *_heap;
    uint32_t _timeouts[2];
    uint32_t _gc_interval_sec;
    Timer _gc_timer;

    enum {
        default_timeout = 300,	   // 5 minutes
        default_guarantee = 5,	   // 5 seconds
        default_gc_interval = 60 * 15 // 15 minutes
    };

    static uint32_t relevant_timeout(const uint32_t timeouts[2]) {
        return timeouts[1] ? timeouts[1] : timeouts[0];
    }

    RewriterEntry *store_flow(RewriterFlow *flow, int input,
                                Map &map, Map *reply_map_ptr = 0);
    inline void unmap_flow(RewriterFlow *flow,
                           Map &map, Map *reply_map_ptr = 0);

    static void gc_timer_hook(Timer *t, void *user_data);

    int parse_input_spec(const String &str, RewriterInput &is,
                         int input_number, ErrorHandler *errh);

    enum {			// < 0 because individual patterns are >= 0
        h_nmappings = -1, h_mapping_failures = -2, h_patterns = -3,
        h_size = -4, h_capacity = -5, h_clear = -6
    };
    static String read_handler(Element *e, void *user_data) CLICK_COLD;
    static int write_handler(const String &str, Element *e, void *user_data, ErrorHandler *errh) CLICK_COLD;
    static int pattern_write_handler(const String &str, Element *e, void *user_data, ErrorHandler *errh) CLICK_COLD;

    friend int RewriterInput::rewrite_flowid(const IPFlowID &flowid,
                                               IPFlowID &rewritten_flowid, Packet *p, int mapid);

private:

    void shift_heap_best_effort(click_jiffies_t now_j);
    bool shrink_heap_for_new_flow(RewriterFlow *flow, click_jiffies_t now_j);
    void shrink_heap(bool clear_all);

    friend class RewriterFlow;

};


class Mapper { public:

    Mapper()				{ }
    virtual ~Mapper()			{ }

    virtual void notify_rewriter(RewriterBase *user, RewriterInput *input,
                                 ErrorHandler *errh);
    virtual int rewrite_flowid(RewriterInput *input,
                               const IPFlowID &flowid,
                               IPFlowID &rewritten_flowid,
                               Packet *p, int mapid);

};


inline int
RewriterInput::rewrite_flowid(const IPFlowID &flowid,
                                IPFlowID &rewritten_flowid,
                                Packet *p, int mapid)
{
    int i;
    switch (kind) {
        case i_nochange:
            return foutput;
        case i_keep:
            rewritten_flowid = flowid;
            return RewriterBase::rw_addmap;
        case i_pattern: {
            HashContainer<RewriterEntry> *reply_map;
            if (likely(mapid == mapid_default))
                reply_map = &reply_element->_map;
            else
                reply_map = reply_element->get_map(mapid);
            i = u.pattern->rewrite_flowid(flowid, rewritten_flowid, *reply_map);
            goto check_for_failure;
        }
        case i_mapper:
            i = u.mapper->rewrite_flowid(this, flowid, rewritten_flowid, p, mapid);
            goto check_for_failure;
        check_for_failure:
            if (i == RewriterBase::rw_drop)
                ++failures;
            return i;
        default:
            return RewriterBase::rw_drop;
    }
}

inline void
RewriterBase::unmap_flow(RewriterFlow *flow, Map &map,
                           Map *reply_map_ptr)
{
    //click_chatter("kill %s", hashkey().s().c_str());
    if (!reply_map_ptr)
        reply_map_ptr = &flow->owner()->reply_element->_map;
    Map::iterator it = map.find(flow->entry(0).hashkey());
    if (it.get() == &flow->entry(0))
        map.erase(it);
    it = reply_map_ptr->find(flow->entry(1).hashkey());
    if (it.get() == &flow->entry(1))
        reply_map_ptr->erase(it);
}

CLICK_ENDDECLS
#endif