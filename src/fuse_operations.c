#include "fuse_operations.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"
#include "data.h"

static const char *serf_str = "Hello serf\n";
static const char *serf_name= "serf";

static int hello_stat(fuse_ino_t ino, struct stat *stbuf)
{
	stbuf->st_ino = ino;
	switch (ino) {
	case 1:
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		break;

	case 2:
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(serf_str);
		break;

	default:
		return -1;
	}
	return 0;
}

void serf_getattr(fuse_req_t req, fuse_ino_t ino,
        struct fuse_file_info *fi)
{
	struct stat stbuf;

	(void) fi;

	memset(&stbuf, 0, sizeof(stbuf));
	if (hello_stat(ino, &stbuf) == -1)
		fuse_reply_err(req, ENOENT);
	else
		fuse_reply_attr(req, &stbuf, 1.0);
}

void serf_lookup(fuse_req_t req, fuse_ino_t parent, const char *name)
{
	struct fuse_entry_param e;

	if (parent != 1 || strcmp(name, serf_name) != 0)
		fuse_reply_err(req, ENOENT);
	else {
		memset(&e, 0, sizeof(e));
		e.ino = 2;
		e.attr_timeout = 1.0;
		e.entry_timeout = 1.0;
		hello_stat(e.ino, &e.attr);

		fuse_reply_entry(req, &e);
	}
}

#define min(x, y) ((x) < (y) ? (x) : (y))

int reply_buf_limited(fuse_req_t req, const char *buf, size_t bufsize, off_t off, size_t maxsize)
{
	if (off < bufsize)
		return fuse_reply_buf(req, buf + off,
				      min(bufsize - off, maxsize));
	else
		return fuse_reply_buf(req, NULL, 0);
}

void serf_readdir(fuse_req_t req, fuse_ino_t ino, size_t size,
			     off_t off, struct fuse_file_info *fi)
{
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
	if (ino != 2)
		fuse_reply_err(req, EISDIR);
	else if ((fi->flags & O_ACCMODE) != O_RDONLY)
		fuse_reply_err(req, EACCES);
	else
		fuse_reply_open(req, fi);
}


void serf_read(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi)
{
	(void) fi;

	assert(ino == 2);
	reply_buf_limited(req, serf_str, strlen(serf_str), off, size);
}
