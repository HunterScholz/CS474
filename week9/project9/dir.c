#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "block.h"
#include "inode.h"
#include "pack.h"

#define SUPERBLOCK_BLOCK 0
#define INODE_MAP_BLOCK  1
#define FREE_MAP_BLOCK   2
#define DIR_ENTRY_SIZE 32

struct directory {
    struct inode *inode;
    unsigned int offset;
};

struct directory_entry {
    unsigned int inode_num;
    char name[16];
};

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

    // Root Directory
    struct inode *new_inode = ialloc();
    int block_num = alloc();

    new_inode->flags = 2;
    new_inode->size = DIR_ENTRY_SIZE * 2;
    new_inode->block_ptr[0] = block_num;
    
    unsigned char block[BLOCK_SIZE] = {0};

    // Add first entry
    write_u16(block+0, new_inode->inode_num);
    strcpy((char *)(block + FREE_MAP_BLOCK), ".");

    // Add second entry
    write_u16(block+DIR_ENTRY_SIZE, new_inode->inode_num);
    strcpy((char *)(block + FREE_MAP_BLOCK + DIR_ENTRY_SIZE), "..");

    bwrite(block_num, block);
    iput(new_inode);
}

struct directory *directory_open(int inode_num){
    struct inode *inode = iget(inode_num);
    if(inode == NULL)
        return NULL;

    struct directory *dir = malloc(sizeof(struct directory));
    dir->inode = inode;
    dir->offset = 0;

    return dir;
}

int directory_get(struct directory *dir, struct directory_entry *ent){
    if(dir->offset >= dir->inode->size)
        return -1;

    unsigned char block[BLOCK_SIZE] = {0};
    int data_block_index = dir->offset / BLOCK_SIZE;
    int data_block_num = dir->inode->block_ptr[data_block_index];

    bread(data_block_num, block);
    int offset_in_block = dir->offset % BLOCK_SIZE;

    ent->inode_num = read_u16(block+offset_in_block);
    strcpy(ent->name, (char *)(block + offset_in_block + FREE_MAP_BLOCK));

    dir->offset += DIR_ENTRY_SIZE;

    return 0;
}

void directory_close(struct directory *d){
    iput(d->inode);
    free(d);
}
