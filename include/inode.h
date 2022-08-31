#ifndef INODE_H
#define INODE_H 

#include <stdint.h>
#include <stdlib.h>

#define BLOCK_SIZE 4096
#define INO_BLOCKS_MAX 8

typedef uint8_t block_t[BLOCK_SIZE];

typedef struct serf_ino {
    mode_t  mode;
    nlink_t nlink;
    block_t block;
    off_t   size;
} serf_ino_t;

#endif
