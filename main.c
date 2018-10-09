/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall  -o main main.c `pkg-config fuse --cflags --libs`

*/

#define FUSE_USE_VERSION 26

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#endif

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include "layout.h"
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif

// static const char *hello_path = "/hello";
static const char *filepath = "/file";
static const char *filename = "file";
static const char *filecontent = "I'm the content of the only file available there\n";


static int vfs_getattr(const char *path, struct stat *stbuf)
{
  memset(stbuf, 0, sizeof(struct stat));

  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
    return 0;
  }

  if (strcmp(path, filepath) == 0) {
    stbuf->st_mode = S_IFREG | 0777;
    stbuf->st_nlink = 1;
    stbuf->st_size = strlen(filecontent);
    return 0;
  }

  return -ENOENT;
}

static int vfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{

  (void) offset;
  (void) fi;

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);
  filler(buf, filename, NULL, 0);
  return 0;
}


static int vfs_mkdir(const char *path, mode_t mode)
{
	int res;

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int vfs_rmdir(const char *path)
{
	int res;

	res = rmdir(path);
	if (res == -1)
		return -errno;

	return 0;
}


static int vfs_open(const char *path, struct fuse_file_info *fi)
{
	int res;

	res = open(path, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int vfs_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	if (strcmp(path, filepath) == 0) {
    size_t len = strlen(filecontent);
    if (offset >= len) {
      return 0;
    }

    if (offset + size > len) {
      memcpy(buf, filecontent + offset, len - offset);
      return len - offset;
    }

    memcpy(buf, filecontent + offset, size);
    return size;
  }

  return -ENOENT;

}



static struct fuse_operations xmp_oper = {
	.getattr	= vfs_getattr,
	.readdir	= vfs_readdir,
	.mkdir		= vfs_mkdir,
	.rmdir		= vfs_rmdir,
	.open		= vfs_open,
	.read		= vfs_read,
#ifdef HAVE_SETXATTR
	
#endif
};

int main(int argc, char *argv[])
{
    super_block* sblock;
    i_bmap* imap;
    d_bmap * dmap;
    initialize_fs(sblock, imap, dmap);
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
