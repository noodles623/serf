#ifndef SERF_DATA_H
#define SERF_DATA_H

#ifndef FUSE_USE_VERSION 
#define FUSE_USE_VERSION 34
#endif

#include <fuse_lowlevel.h>

typedef struct dirbuf {
    char *p;
    size_t size;
} dirbuf_t;

void dirbuf_add(fuse_req_t req, dirbuf_t *b, const char *name, fuse_ino_t ino);

#endif
