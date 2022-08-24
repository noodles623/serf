#ifndef FS_H
#define FS_H

#include <stddef.h>
#include <sys/types.h>

#include "dir.h"

#define MAX_INODES 4

typedef enum err {
    err_ISDIR,
    err_ACCES,
    err_OK
} err_t;

typedef struct fs {
    inode_t inodes[MAX_INODES];
} fs_t;

err_t open(fs_t *fs, uint64_t ino);

void read(fs_t *fs, uint64_t ino, char *buf, size_t size, off_t off);

void write(fs_t *fs, uint64_t ino, const char *buf, size_t, off_t off);

#endif
