#include <unistd.h>
#include "free.h"
#include "block.h"

// allocate a previously-free inode in the inode map
int ialloc(void){
    unsigned char buf[BLOCK_SIZE] = {0};

    unsigned char *block = bread(INODE_MAP_BLOCK, buf);

    int num = find_free(block);
    if(num == -1)
        return -1;
    set_free(block, num, 1);

    bwrite(INODE_MAP_BLOCK, block);
    return num;
}
