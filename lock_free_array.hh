#ifndef LOCK_FREE_ARRAY_HH
#define LOCK_FREE_ARRAY_HH

#include <click/config.h>
#include <click/element.hh>

#define DEFAULT_SIZE         8
#define DEFAULT_HANDLER_SIZE 8

CLICK_DECLS

class LockFreeArray : public Element {
public:
    int array[DEFAULT_SIZE];

    LockFreeArray ();

    ~LockFreeArray();

    const char *class_name() const { return "LockFreeArray"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *);

    void add_handlers();

    inline void increment(int index) {
        ++ array[index];
    }

private:
    static String read_handler(Element*, void*);
};

CLICK_ENDDECLS
#endif