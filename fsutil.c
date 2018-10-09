#include "layout.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

super_block *initialize_superblock()
{
    super_block *sblock;
    sblock = (super_block*)malloc(sizeof(super_block));
    memset(sblock, 0, sizeof(super_block));
    sblock->no_of_inodes = INODE_NUM;
    sblock->no_of_datablocks = DATA_NUM;
    sblock->inode_off = 12288;           //3 blocks reserved, each block 4KB
    sblock->fstype = 1;         //1 - very simple file system
    printf("\nsblock->fstype: %d",sblock->fstype);
    return sblock;
}

i_bmap* initialize_ibmap()
{
    i_bmap *ibmap = (i_bmap*)malloc(sizeof(i_bmap));
    memset(ibmap, 0, sizeof(i_bmap));
    printf("\nimap[0]: %d",ibmap->map[0]);
    return ibmap;
}

d_bmap* initialize_dbmap()
{
    d_bmap *dbmap = (d_bmap*)malloc(sizeof(d_bmap));
    memset(dbmap, 0, sizeof(d_bmap));
    printf("\nDmap[0]: %d",dbmap->map[0]);
    return dbmap;
}

inode*  create_root_inode(ino_t ID, int FILE_TYPE, mode_t MODE, blkcnt_t BLOCKS)
{
    inode *root_inode = (inode*)malloc(sizeof(inode));
    memset(root_inode, 0, sizeof(inode));
    root_inode->id = ID;
    root_inode->ftype = FILE_TYPE;
    root_inode->mode = MODE;
    root_inode->blocks = BLOCKS;
    root_inode->c_time = time(0);
    return root_inode;
}

void initialize_fs(super_block* sblock, i_bmap* imap, d_bmap *dmap)
{
    sblock = initialize_superblock();
    imap = initialize_ibmap();
    dmap = initialize_dbmap();
}
