#pragma once

#include <click/config.h>
#include <click/element.hh>

#define DEFAULT_SIZE 65536

CLICK_DECLS

enum { H_COUNT };

class LockFreeArray : public Element {
public:
    int counters[DEFAULT_SIZE];

    LockFreeArray ();

    ~LockFreeArray();

    const char *class_name() const { return "LockFreeArray"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    void add_handlers();

private:
    String read_handler(Element* e, void* thunk) {
        LockFreeArray *c = (LockFreeArray *)e;
        String res;
        switch ((intptr_t)thunk) {
            case H_COUNT:
                for(int i = 0; i < DEFAULT_SIZE / 10000; ++i) {
                    res += String(c->counters[i]);
                    res += ",";
                }//for
                return res;

            default:
                return "<error>";
        }
    }
};

CLICK_ENDDECLS
