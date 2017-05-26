#include <click/config.h>
#include <click/args.hh>
#include "FTLoggerElement.hh"
CLICK_DECLS

FTLoggerElement::FTLoggerElement() { }

FTLoggerElement::~FTLoggerElement() { }

int FTLoggerElement::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh)
                .read(CONF_ID, _id)
                .read(CONF_VLAN_ID, _vlanId)
                .read(CONF_FAILURE_COUNT, _failureCount)
                .complete() < 0)
        return -1;

    //LOG("Configuration of state element: id: %d, vlanId: %d, f: %d\n", _id, _vlanId, _failureCount);

    return 0;
}

void FTLoggerElement::push(int port, Packet *p) {
    LOG("------------------------------");
    LOG("Begin FTLoggerElement");

    _pkt_time[FTAppenderElement::getPacketId(p)] = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    output(0).push(p);

    LOG("End FTLoggerElement");
    LOG("------------------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTLoggerElement)