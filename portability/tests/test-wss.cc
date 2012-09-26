/* -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*- */
// vim: ft=cpp:expandtab:ts=8:sw=4:softtabstop=4:
#ident "$Id$"
#ident "Copyright (c) 2007-2012 Tokutek Inc.  All rights reserved."
#ident "The technology is licensed by the Massachusetts Institute of Technology, Rutgers State University of New Jersey, and the Research Foundation of State University of New York at Stony Brook under United States of America Serial No. 11/760379 and to the patents and/or patent applications resulting from it."
#include <stdio.h>
#include <stdlib.h>
#include <toku_assert.h>
#include <string.h>
#include <toku_stdint.h>
#include <toku_os.h>

const int nbuffers = 1000;
const int buffersize = 1024*1024;

static void do_mallocs(void) {
    int i;
    void *vp[nbuffers];
    for (i=0; i<nbuffers; i++) {
        int nbytes = buffersize;
        vp[i] = malloc(nbytes);
        memset(vp[i], 0, nbytes);
    }
    for (i=0; i<nbuffers; i++)
        free(vp[i]);
}

int main(int argc, char *argv[]) {
    int verbose = 0;
    int i;
    for (i=1; i<argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
            continue;
        }
        if (strcmp(argv[i], "-q") == 0) {
            verbose = 0;
            continue;
        }
    }

    int64_t rss;
    int r = toku_os_get_max_rss(&rss);
    invariant_zero(r);
    if (verbose) printf("%" PRId64 "\n", rss);
    assert(rss < nbuffers*buffersize);
    do_mallocs();
    r = toku_os_get_max_rss(&rss);
    invariant_zero(r);
    if (verbose) printf("%" PRId64 "\n", rss);
    assert(rss > nbuffers*buffersize);

    return 0;
}

    
