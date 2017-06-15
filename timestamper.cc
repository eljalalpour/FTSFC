// -*- c-basic-offset: 4 -*-
/*
 * Timestamper.{cc,hh} -- accumulate cycle counter deltas
 * Eddie Kohler
 *
 * Copyright (c) 2002 International Computer Science Institute
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
#include "timestamper.hh"
#include <click/glue.hh>
#include <vector>
#include <csignal>
#include <fstream>
CLICK_DECLS

using std::vector;

vector<Timestamper*> _global_ts;

void handle_signal(int signal) {
	for (int i = 0; i < _global_ts.size(); ++i) {
		_global_ts[i]->write_to_file();
	}
	_global_ts.clear();
	exit(signal);
}

Timestamper::Timestamper()
{
	_global_ts.push_back(this);
}

Timestamper::~Timestamper()
{
    //printf("crash called %f\n", _usec_accum);
    
}

void
Timestamper::write_to_file() 
{
	//ofstream ofile(log_path.c_str(), std::ofstream::out | std::ofstream::app);
	std::ofstream ofile("latency", std::ofstream::out | std::ofstream::app);
	
	while(_latency.size()){
		ofile << _latency.back() << "\n";
		_latency.pop_back();
	}//while
	ofile.close();
}

int
Timestamper::initialize(ErrorHandler *)
{
    _usec_accum = 0;
    _usec_base = 0;
    _count = 0;
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    return 0;
}
/*
inline Packet *
Timestamper::simple_action(Packet *p)
{
    _usec_accum += (Timestamp::now() - p->timestamp_anno()).doubleval();
    _count++;
    return p;
}
*/
void
Timestamper::push(int port, Packet *pkt)
{
    switch (port){
	case 0:
	    _usec_base = Timestamp::now().doubleval();
	    output(0).push(pkt);
	    break;
	case 1:
	    _latency.push_back(Timestamp::now().doubleval() - _usec_base);
	    _usec_accum += _latency.back();
	    _count++;
	    output(1).push(pkt);
	    break;
    }	
}

String
Timestamper::read_handler(Element *e, void *thunk)
{
    Timestamper *ta = static_cast<Timestamper *>(e);
    int which = reinterpret_cast<intptr_t>(thunk);
    switch (which) {
      case 0:
	return String(ta->_count);
      case 1:
	return String(ta->_usec_accum);
      case 2:
	return String(ta->_usec_accum / ta->_count);
      default:
	return String();
    }
}

int
Timestamper::reset_handler(const String &, Element *e, void *, ErrorHandler *)
{
    Timestamper *ta = static_cast<Timestamper *>(e);
    ta->_usec_accum = 0;
    ta->_count = 0;
    return 0;
}

void
Timestamper::add_handlers()
{
    add_read_handler("count", read_handler, 0);
    add_read_handler("time", read_handler, 1);
    add_read_handler("average_time", read_handler, 2);
    add_write_handler("reset_counts", reset_handler, 0, Handler::f_button);
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel int64)
EXPORT_ELEMENT(Timestamper)
