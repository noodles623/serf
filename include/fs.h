#ifndef FS_H
#define FS_H

#include <stddef.h>
#include <sys/types.h>

#include "inode.h"

#define MAX_INODES 4

typedef enum err {
    err_ISDIR,
    err_ACCES,
    err_OK
} err_t;

typedef struct fs {
    serf_ino_t inodes[MAX_INODES]; 
} fs_t;

#endif
