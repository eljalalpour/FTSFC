#ifndef ATOMIC_ARRAY_HH
#define ATOMIC_ARRAY_HH

#include <atomic>
#include <click/config.h>
#include <click/element.hh>

#define DEFAULT_SIZE         8
#define DEFAULT_HANDLER_SIZE 8

CLICK_DECLS

class AtomicArray : public Element {
public:
    std::atomic_int counters[DEFAULT_SIZE];

    AtomicArray ();

    ~AtomicArray();

    const char *class_name() const { return "AtomicArray"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    void add_handlers();

private:
    static String read_handler(Element* e, void* thunk);
};

CLICK_ENDDECLS
#endif