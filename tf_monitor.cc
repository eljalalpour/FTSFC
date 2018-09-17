#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "tf_monitor.hh"

CLICK_DECLS

TFMonitor::TFMonitor  () : _trans_init(false) { };

TFMonitor::~TFMonitor () { };

int TFMonitor::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .read("FOR", _for_count)
                .complete() < 0)
        return -1;

    LOG("TFMonitor index is %d!\n", _index);

    return 0;
}

void TFMonitor::_init_transmitter() {
    if (!_trans_init) {
        Router *r = this->router();
        _trans = (Transmitter *)(r->find("trans"));

        _trans_init = true;
    }//if
}

Packet *TFMonitor::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin TFMonitor");

    _init_transmitter();

    (*_trans).inoperation[_index] ++;
    _trans->send();

    for (int i = 0; i < _for_count; ++i) {
        rand();
    }//for

    DEBUG("End TFMonitor");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TFMonitor)
