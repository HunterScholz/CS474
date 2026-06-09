#ifndef DIR_H
#define DIR_H

#define SUPERBLOCK_BLOCK 0
#define INODE_MAP_BLOCK  1
#define FREE_MAP_BLOCK   2
#define DIR_ENTRY_SIZE 32
#define ROOT_INODE_NUM 0

struct directory {
    struct inode *inode;
    unsigned int offset;
};

struct directory_entry {
    unsigned int inode_num;
    char name[16];
};

int mkfs(void);
struct directory *directory_open(int inode_num);
int directory_get(struct directory *dir, struct directory_entry *ent);
void directory_close(struct directory *d);
struct inode *namei(char *path);
int directory_make(char *path);

#endif