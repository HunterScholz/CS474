#ifndef MKFS_H
#define MKFS_H

#define SUPERBLOCK_BLOCK 0
#define INODE_MAP_BLOCK  1
#define FREE_MAP_BLOCK   2

int mkfs(void);

#endif