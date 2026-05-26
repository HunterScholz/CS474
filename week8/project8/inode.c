#include <unistd.h>
#include "free.h"
#include "block.h"
#include "pack.h"


#define BLOCK_SIZE 4096
#define INODE_SIZE 64
#define INODE_FIRST_BLOCK 3

#define INODES_PER_BLOCK (BLOCK_SIZE / INODE_SIZE)

// int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
// int block_offset = inode_num % INODES_PER_BLOCK;
// int block_offset_bytes = block_offset * INODE_SIZE;
// int flags = read_u8(block + block_offset_bytes + 7);

#define MAX_SYS_OPEN_FILES 64
#define INODE_PTR_COUNT 16

struct inode {
    unsigned int size;
    unsigned short owner_id;
    unsigned char permissions;
    unsigned char flags;
    unsigned char link_count;
    unsigned short block_ptr[INODE_PTR_COUNT];

    unsigned int ref_count;  // in-core only
    unsigned int inode_num;  // in-core only
};

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};

struct inode *incore_find_free(void){
    for(int i = 0; i < MAX_SYS_OPEN_FILES; i++){
        if(incore[i].ref_count == 0)
            return &incore[i];
    }
    return NULL;
}

struct inode *incore_find(unsigned int inode_num){
   for(int i = 0; i < MAX_SYS_OPEN_FILES; i++){
        if(incore[i].ref_count != 0 
        && incore[i].inode_num == inode_num)
            return &incore[i];
    }
    return NULL;
}

void incore_free_all(void){
    for(int i = 0; i < MAX_SYS_OPEN_FILES; i++)
        incore[i].ref_count = 0;
}

void read_inode(struct inode *in, int inode_num){
    int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    int block_offset = inode_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;

    unsigned char buf[BLOCK_SIZE];
    unsigned char *block = bread(block_num, buf);

    in->size = read_u32(block + block_offset_bytes);
    in->owner_id = read_u16(block + block_offset_bytes + 4);
    in->permissions = read_u8(block + block_offset_bytes + 6);
    in->flags = read_u8(block + block_offset_bytes + 7);
    in->link_count = read_u8(block + block_offset_bytes + 8);

    for(int i = 0; i < INODE_PTR_COUNT; i++)
        in->block_ptr[i] = read_u16(block + block_offset_bytes + 9 + (i*2));
}

void write_inode(struct inode *in){
    int inode_num = in->inode_num;

    int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    int block_offset = inode_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;

    unsigned char buf[BLOCK_SIZE];
    unsigned char *block = bread(block_num, buf);

    write_u32(block + block_offset_bytes, in->size);
    write_u16(block + block_offset_bytes + 4, in->owner_id);
    write_u8(block + block_offset_bytes + 6, in->permissions);
    write_u8(block + block_offset_bytes + 7, in->flags);
    write_u8(block + block_offset_bytes + 8, in->link_count);

    for(int i = 0; i < INODE_PTR_COUNT; i++)
        write_u16(block + block_offset_bytes + 9 + (i*2), in->block_ptr[i]);

    bwrite(block_num, block);
}

struct inode *iget(int inode_num){
    struct inode *inode;
    inode = incore_find(inode_num);
    if(inode != NULL){
        inode->ref_count++;
        return inode;
    }

    inode = incore_find_free();
    if(inode == NULL)
        return NULL;

    read_inode(inode, inode_num);
    inode->ref_count = 1;
    inode->inode_num = inode_num;

    return inode;
}

void iput(struct inode *in){
    if(in->ref_count == 0)
        return;

    in->ref_count--;

    if(in->ref_count == 0)
        write_inode(in);
}

struct inode *ialloc(void){
    unsigned char buf[BLOCK_SIZE] = {0};

    unsigned char *block = bread(INODE_MAP_BLOCK, buf);

    int num = find_free(block);
    if(num == -1)
        return NULL;

    struct inode *inode = iget(num);
    if(inode == NULL)
        return NULL;

    set_free(block, num, 1);
    bwrite(INODE_MAP_BLOCK, block);

    inode->size = 0;
    inode->owner_id = 0;
    inode->permissions = 0;
    inode->flags = 0;
    for(int i = 0; i < INODE_PTR_COUNT; i++)
        inode->block_ptr[i] = 0;
    inode->inode_num = num;

    write_inode(inode);
    return inode;
}
