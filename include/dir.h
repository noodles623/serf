#ifndef SERF_DIR_H
#define SERF_DIR_H

#include "inode.h"

#define FNAME_CHARS_MAX 256

typedef struct file {
    char name[FNAME_CHARS_MAX];
    uint32_t size;
    inode_t ino;
} file_t;

typedef struct dir {
    file_t *files;
    uint32_t size;
} dir_t;

#endif
