#ifndef FUSE_OPERATIONS_H 
#define FUSE_OPERATIONS_H

#ifndef FUSE_USE_VERSION 
#define FUSE_USE_VERSION 34
#endif

#include <fuse_lowlevel.h>

void serf_getattr(fuse_req_t req, fuse_ino_t ino,
        struct fuse_file_info *fi);

void serf_lookup(fuse_req_t req, fuse_ino_t parent, const char *name);

void serf_readdir(fuse_req_t req, fuse_ino_t ino, size_t size,
			     off_t off, struct fuse_file_info *fi);

void serf_open(fuse_req_t req, fuse_ino_t ino,
			  struct fuse_file_info *fi);

void serf_read(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi);

#endif
