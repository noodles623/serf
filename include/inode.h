#ifndef INODE_H
#define INODE_H 

#include <stdint.h>
#include <stdbool.h>

#define BLOCK_SIZE 4096
#define INO_BLOCKS_MAX 8

typedef uint8_t block_t[BLOCK_SIZE];

typedef enum inode_type {
    inode_FILE,
    inode_DIR
} inode_type_t;

typedef struct inode {
    uint64_t num;
    inode_type_t type;
    bool exists;
    block_t blocks[INO_BLOCKS_MAX];
    uint32_t size;
} inode_t;

#endif
