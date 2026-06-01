#include <unistd.h>
#include <stdio.h>
#include "image.h"
#include "free.h"

#define INODE_MAP_BLOCK 1
#define FREE_MAP_BLOCK  2
#define BLOCK_SIZE 4096

unsigned char *bread(int block_num, unsigned char *block){
    if(lseek(image_fd, block_num * BLOCK_SIZE, SEEK_SET) == -1){
        perror("lseek");
        return NULL;
    }

    ssize_t bytes = read(image_fd, block, BLOCK_SIZE);
    if(bytes != BLOCK_SIZE){
        perror("read");
        return NULL;
    }

    return block;
}

void bwrite(int block_num, unsigned char *block){
    if (lseek(image_fd, block_num * BLOCK_SIZE, SEEK_SET) == -1){
        perror("lseek");
        return;
    }

    ssize_t bytes = write(image_fd, block, BLOCK_SIZE);
    if(bytes != BLOCK_SIZE){
        perror("write");
        return;
    }
}

// allocate a previously-free data block from the block map
int alloc(void){
    unsigned char buf[BLOCK_SIZE] = {0};

    unsigned char *block = bread(FREE_MAP_BLOCK, buf);

    int num = find_free(block);
    if(num == -1)
        return -1;
    set_free(block, num, 1);

    bwrite(FREE_MAP_BLOCK, block);
    return num;
}