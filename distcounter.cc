#include <click/config.h>
#include "distcounter.hh"
#include <click/error.hh>
#include <click/confparse.hh>
#include <click/args.hh>
#include <map>
#include <iostream>

CLICK_DECLS

distCounter::distCounter()
{
}

distCounter::~distCounter()
{
    std::map<int, uint64_t>::iterator it = counter.begin();
    for (;it != counter.end(); it++)
        std::cout << it->first << " : " << it->second << std::endl;
}
/*
int
distCounter::configure(Vector<String> &, ErrorHandler *)
{

}
*/
Packet *
distCounter::simple_action(Packet *p)
{
    std::map<int, uint64_t>::iterator it;
    if( (it = counter.find( p->length() )) == counter.end() )
        counter.insert(std::make_pair(p->length(), 1));
    else
        it->second += 1;
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(distCounter)
