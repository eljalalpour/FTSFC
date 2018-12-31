/*
 * iprwpatterns.{cc,hh} -- stores shared IPRewriter patterns
 * Eddie Kohler
 *
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2009-2010 Meraki, Inc.
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
#include "rwpatterns.hh"
#include "rwpattern.hh"
#include <click/confparse.hh>
#include <click/router.hh>
#include <click/error.hh>
#include <click/nameinfo.hh>
CLICK_DECLS

RewriterPatterns::RewriterPatterns()
{
}

RewriterPatterns::~RewriterPatterns()
{
}

int
RewriterPatterns::configure(Vector<String> &conf, ErrorHandler *errh)
{
    void *&patterns_attachment = router()->force_attachment("RewriterPatterns");
    if (!patterns_attachment)
        patterns_attachment = new Vector<RewriterPattern *>;
    Vector<RewriterPattern *> *patterns =
            static_cast<Vector<RewriterPattern *> *>(patterns_attachment);

    for (int i = 0; i < conf.size(); ++i) {
        String name = cp_shift_spacevec(conf[i]);
        if (!name)
            continue;

        int32_t x;
        if (NameInfo::query_int(NameInfo::T_IPREWRITER_PATTERN, this,
                                name, &x)) {
            errh->error("pattern %<%s%> already defined", name.c_str());
            continue;
        }

        Vector<String> words;
        cp_spacevec(conf[i], words);
        RewriterPattern *p;
        if (!RewriterPattern::parse(words, &p, this, errh))
            continue;

        p->use();
        patterns->push_back(p);
        NameInfo::define_int(NameInfo::T_IPREWRITER_PATTERN, this,
                             name, patterns->size() - 1);
    }

    return errh->nerrors() ? -1 : 0;
}

void
RewriterPatterns::cleanup(CleanupStage)
{
    void *&patterns_attachment = router()->force_attachment("RewriterPatterns");
    if (patterns_attachment) {
        Vector<RewriterPattern *> *patterns =
                static_cast<Vector<RewriterPattern *> *>(patterns_attachment);
        for (int i = 0; i < patterns->size(); ++i)
            (*patterns)[i]->unuse();
        delete patterns;
        patterns_attachment = 0;
    }
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(RewriterPattern)
EXPORT_ELEMENT(RewriterPatterns)