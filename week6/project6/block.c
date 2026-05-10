#include <unistd.h>
#include <stdio.h>
#include "image.h"

#define BLOCK_SIZE 4096

unsigned char *bread(int block_num, unsigned char *block){
    if(lseek(image_fd, block_num * BLOCK_SIZE, SEEK_SET) == -1){
        perror("lseek");
        return NULL;
    }

    ssize_t bytes = read(image_fd, block, BLOCK_SIZE);
    if(bytes > BLOCK_SIZE){
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
    if(bytes > BLOCK_SIZE){
        perror("write");
        return;
    }
}