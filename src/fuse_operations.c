#include "fuse_operations.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"
#include "dir.h"
#include "data.h"

static const char *serf_str  = "Hello serf\n";
static const char *serf_name = "serf";

fs_t fs;

void serf_init(void *userdata, struct fuse_conn_info *conn)
{
    printf("%s\n", __func__);
    dir_t dir;
    memset(&dir, 0, sizeof(dir_t));


    strncpy(&dir.file.name[0], serf_name, 4);
    dir.file.size = 4;

    dir.file.stbuf.st_ino   = 2;
    dir.file.stbuf.st_nlink = 1;
    dir.file.stbuf.st_mode  = S_IFREG | 0444;
    dir.file.stbuf.st_size  = strlen(serf_str);

    fs.inodes[2].mode  = S_IFREG | 0444;
    fs.inodes[2].nlink = 1;
    fs.inodes[2].size  = dir.file.stbuf.st_size; 

    fs.inodes[1].mode  = S_IFDIR | 0755;
    fs.inodes[1].nlink = 2;

    memcpy(&fs.inodes[1].block, &dir, sizeof(dir_t));
    memcpy(&fs.inodes[2].block, serf_str, fs.inodes[2].size);
}

static int serf_stat(fuse_ino_t ino, struct stat *stbuf)
{
    if (ino > MAX_INODES) {
        return -1;
    }

	stbuf->st_ino = ino;
    stbuf->st_mode = fs.inodes[ino].mode;
    stbuf->st_nlink= fs.inodes[ino].nlink;
    stbuf->st_size = fs.inodes[ino].size;

	return 0;
}

void serf_getattr(fuse_req_t req, fuse_ino_t ino,
        struct fuse_file_info *fi)
{
    printf("%s\n", __func__);
    struct stat stbuf;

	(void) fi;

	memset(&stbuf, 0, sizeof(stbuf));
	if (serf_stat(ino, &stbuf) == -1)
		fuse_reply_err(req, ENOENT);
	else
		fuse_reply_attr(req, &stbuf, 1.0);
}

void serf_lookup(fuse_req_t req, fuse_ino_t parent, const char *name)
{
	printf("%s\n", __func__);
    struct fuse_entry_param e;

	if (parent != 1 || strcmp(name, serf_name) != 0)
		fuse_reply_err(req, ENOENT);
	else {
		memset(&e, 0, sizeof(e));
		e.ino = 2;
		e.attr_timeout = 1.0;
		e.entry_timeout = 1.0;
		serf_stat(e.ino, &e.attr);

		fuse_reply_entry(req, &e);
	}
}

#define min(x, y) ((x) < (y) ? (x) : (y))

int reply_buf_limited(fuse_req_t req, const char *buf, size_t bufsize, off_t off, size_t maxsize)
{
	if (off < bufsize) {
        printf("True\n");
		return fuse_reply_buf(req, buf + off,
				      min(bufsize - off, maxsize));
    }
	else {
        printf("False\n");
		return fuse_reply_buf(req, NULL, 0);
    }
}

void serf_readdir(fuse_req_t req, fuse_ino_t ino, size_t size,
			     off_t off, struct fuse_file_info *fi)
{
    printf("%s\n", __func__);
	(void) fi;

	if (ino != 1)
		fuse_reply_err(req, ENOTDIR);
	else {
		dirbuf_t b;

		memset(&b, 0, sizeof(b));
		dirbuf_add(req, &b, ".", 1);
		dirbuf_add(req, &b, "..", 1);
		dirbuf_add(req, &b, serf_name, 2);
		reply_buf_limited(req, b.p, b.size, off, size);
		free(b.p);
	}
}

void serf_open(fuse_req_t req, fuse_ino_t ino,
			  struct fuse_file_info *fi)
{
    printf("%s\n", __func__);
    if (ino >= MAX_INODES) {
        fuse_reply_err(req, EACCES);
    } 
    else if (S_ISDIR((fs.inodes[ino].mode))) {
        fuse_reply_err(req, EISDIR);
    }
    else {
        fuse_reply_open(req, fi);
    }
}

void serf_read(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi)
{
    printf("%s\n", __func__);
    if (ino >= MAX_INODES) {
        fuse_reply_err(req, EACCES);
    }
    else if (S_ISDIR((fs.inodes[ino].mode))) {
        fuse_reply_err(req, EISDIR);
    }
    else {
        const char *buf = (const char*)&fs.inodes[ino].block[0];
        reply_buf_limited(req, buf, fs.inodes[ino].size, off, size);
        printf("%s", buf);
    }
}

void serf_write(fuse_req_t req, fuse_ino_t ino, const char *buf, size_t size, off_t off, struct fuse_file_info *fi) {
    printf("%s\n", __func__);
    if (ino >= MAX_INODES) {
        fuse_reply_err(req, EACCES);
    }
    else if (S_ISDIR((fs.inodes[ino].mode))) {
        fuse_reply_err(req, EISDIR);
    }
    else {
        char *dest = (char*)&fs.inodes[ino].block[0];
        strncpy(dest+off, buf, size);
        fs.inodes[ino].size = size + off;
        fuse_reply_write(req, size);
    }
}

