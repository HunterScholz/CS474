#ifndef BLOCK_H
#define BLOCK_H

#define INODE_MAP_BLOCK 1
#define FREE_MAP_BLOCK  2
#define BLOCK_SIZE 4096

unsigned char *bread(int block_num, unsigned char *block);
void bwrite(int block_num, unsigned char *block);
int alloc(void);

#endif