#ifndef SERF_DIR_H
#define SERF_DIR_H

#include <stdlib.h>

#define FNAME_CHARS_MAX 256

typedef struct file {
    char name[FNAME_CHARS_MAX];
    uint32_t size;
    struct stat stbuf;
} file_t;

typedef struct dir {
    file_t file;
} dir_t;

#endif
