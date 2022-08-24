#include "fs.h"

#include <string.h>

#define min(x, y) ((x) < (y) ? (x) : (y))

err_t open(fs_t *fs, uint64_t ino) 
{
    if (ino >= MAX_INODES) {
        return err_ACCES;
    }
    if (fs->inodes[ino].type == inode_DIR) {
        return err_ISDIR;
    }
    return err_OK;
}

void read(fs_t *fs, uint64_t ino, char *buf, size_t size, off_t off)
{
    block_t *blocks = fs->inodes[ino].blocks;
    uint32_t bp = off / BLOCK_SIZE;
    size_t to_cpy = min(size, BLOCK_SIZE - size);
    // Copy first blocks
    memcpy(buf, blocks[bp], to_cpy);
}

void write(fs_t *fs, uint64_t ino, const char *buf, size_t, off_t off)
{
    block_t *blocks = fs->inodes[ino].blocks;
    uint32_t bp = off / BLOCK_SIZE;
    size_t to_write = min(size, BLOCK_SIZE - size);
    // Write first blocks
    memcpy(blocks[bp], buf, to_write);
}
