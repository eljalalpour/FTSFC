#ifndef CLICK_BURNER_HH
#define CLICK_BURNER_HH
#include <click/element.hh>
CLICK_DECLS

class Burner : public Element { public:
    Burner();
    ~Burner();

    const char *class_name() const { return "Burner"; }
    const char *port_count() const { return PORTS_1_1; }

    Packet *simple_action(Packet *);
    
    int configure(Vector<String> &conf, ErrorHandler *errh);

    private:
    unsigned int _cycle;
};


CLICK_ENDDECLS
#endif
