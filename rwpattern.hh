// -*- mode: c++; c-basic-offset: 4 -*-
#ifndef CLICK_RW_PATTERN_HH
#define CLICK_RW_PATTERN_HH
#include <click/element.hh>
#include <click/hashcontainer.hh>
#include <click/ipflowid.hh>
CLICK_DECLS
class RewriterFlow;
class RewriterEntry;
class RewriterInput;

class RewriterPattern { public:

    RewriterPattern(const IPAddress &saddr, int sport,
                      const IPAddress &daddr, int dport,
                      bool is_napt, bool sequential, bool same_first,
                      uint32_t variation);
    static bool parse(const Vector<String> &words, RewriterPattern **result,
                      Element *context, ErrorHandler *errh);
    static bool parse_ports(const Vector<String> &words, RewriterInput *input,
                            Element *model, ErrorHandler *errh);
    static bool parse_with_ports(const String &str, RewriterInput *input,
                                 Element *context, ErrorHandler *errh);

    void use() {
        _refcount++;
    }
    void unuse() {
        if (--_refcount <= 0)
            delete this;
    }

    operator bool() const {
        return _saddr || _sport || _daddr || _dport;
    }
    IPAddress daddr() const {
        return _daddr;
    }

    int rewrite_flowid(const IPFlowID &flowid, IPFlowID &rewritten_flowid,
                       const HashContainer<RewriterEntry> &reply_map);

    String unparse() const;

private:

    IPAddress _saddr;
    int _sport;			// net byte order
    IPAddress _daddr;
    int _dport;			// net byte order

    uint32_t _variation_top;
    uint32_t _next_variation;

    bool _is_napt;
    bool _sequential;
    bool _same_first;

    int _refcount;

    RewriterPattern(const RewriterPattern&);
    RewriterPattern& operator=(const RewriterPattern&);

};

CLICK_ENDDECLS
#endif