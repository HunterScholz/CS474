#include <unistd.h>
#include "block.h"

#define SUPERBLOCK_BLOCK 0
#define INODE_MAP_BLOCK  1
#define FREE_MAP_BLOCK   2

void mkfs(void){
    unsigned char empty[BLOCK_SIZE] = {0};

    bwrite(SUPERBLOCK_BLOCK, empty);
    bwrite(INODE_MAP_BLOCK, empty);
    bwrite(3, empty);
    bwrite(4, empty);
    bwrite(5, empty);
    bwrite(6, empty);

    unsigned char free_map[BLOCK_SIZE] = {0};
    free_map[0] = 0x7F;

    bwrite(FREE_MAP_BLOCK, free_map);
}