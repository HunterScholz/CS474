#include "image.h"
#include "block.h"
#include "free.h"
#include "inode.h"
#include "dir.h"
#include "ls.h"

#include "ctest.h"
#include <string.h>

void test_image(void){
    CTEST_ASSERT(image_open("foo.img", 1) != -1, "test image open");
    CTEST_ASSERT(image_close() == 0, "test image close");
}

void test_rw(void){
    image_open("foo.img", 1);

    unsigned char read_buf[BLOCK_SIZE] = {0};
    unsigned char write_buf[BLOCK_SIZE] = {0};

    strcpy((char *)write_buf, "hello, world");
    bwrite(3, write_buf);

    CTEST_ASSERT(
        strcmp((const char *)bread(3, read_buf), "hello, world") == 0, 
        "test writing to a block and reading"
    );
}

void test_free(void){
    unsigned char block[BLOCK_SIZE] = {0};

    set_free(block, 0, 1); // [00000001]
    CTEST_ASSERT(block[0] == 1, "set free first bit");

    set_free(block, 1, 1); // [00000011]
    CTEST_ASSERT(block[0] == 3, "set free second bit");

    CTEST_ASSERT(find_free(block) == 2, "test find first free bit");
}

void test_ialloc(void){
    unsigned char clear[BLOCK_SIZE] = {0};
    bwrite(INODE_MAP_BLOCK, clear);

    incore_free_all();

    struct inode *a = ialloc();
    struct inode *b = ialloc();
    struct inode *c = ialloc();

    CTEST_ASSERT(a != NULL, "ialloc inode 0 exists");
    CTEST_ASSERT(b != NULL, "ialloc inode 1 exists");
    CTEST_ASSERT(c != NULL, "ialloc inode 2 exists");

    CTEST_ASSERT(a->inode_num == 0, "ialloc inode 0");
    CTEST_ASSERT(b->inode_num == 1, "ialloc inode 1");
    CTEST_ASSERT(c->inode_num == 2, "ialloc inode 2");

    // Fill block with 1s
    unsigned char buf[BLOCK_SIZE];
    memset(buf, 0xFF, BLOCK_SIZE);
    bwrite(INODE_MAP_BLOCK, buf);

    CTEST_ASSERT(ialloc() == NULL, "ialloc fails when full");
}

void test_alloc(void){
    unsigned char clear[BLOCK_SIZE] = {0};
    bwrite(FREE_MAP_BLOCK, clear);

    CTEST_ASSERT(alloc() == 0, "alloc inode 0");
    CTEST_ASSERT(alloc() == 1, "alloc inode 1");
    CTEST_ASSERT(alloc() == 2, "alloc inode 2");

    // Fill block with 1s
    unsigned char buf[BLOCK_SIZE];
    memset(buf, 0xFF, BLOCK_SIZE);
    bwrite(FREE_MAP_BLOCK, buf);

    CTEST_ASSERT(alloc() == -1, "alloc fails when full");
}

void test_incore(void){
    incore_free_all();

    struct inode *inode = incore_find_free();
    CTEST_ASSERT(inode != NULL, "found free inode");

    inode->inode_num = 42;
    inode->ref_count = 1;
    inode->size = 1234;

    struct inode *found = incore_find(42);

    CTEST_ASSERT(found != NULL, "found inode by number");
    CTEST_ASSERT(found == inode, "found same inode");
    CTEST_ASSERT(found->inode_num == 42, "inode number preserved");
    CTEST_ASSERT(found->size == 1234, "inode data preserved");
    
    incore_free_all();
}

void test_rw_inode(void){
    struct inode out = {0};

    out.inode_num = 0;
    out.size = 1234;
    out.owner_id = 22;
    out.permissions = 6;
    out.flags = 1;
    out.link_count = 3;

    write_inode(&out);
    struct inode in = {0};
    read_inode(&in, 0);

    CTEST_ASSERT(in.size == 1234, "inode size");
    CTEST_ASSERT(in.owner_id == 22, "inode owner");
    CTEST_ASSERT(in.permissions == 6, "inode permissions");
    CTEST_ASSERT(in.flags == 1, "inode flags");
    CTEST_ASSERT(in.link_count == 3, "inode link count");
}

void test_iget(void){
    incore_free_all();

    struct inode *a = iget(42);

    CTEST_ASSERT(a != NULL, "iget allocates inode");
    CTEST_ASSERT(a->inode_num == 42, "inode number set");
    CTEST_ASSERT(a->ref_count == 1, "refcount initialized");

    struct inode *b = iget(42);

    CTEST_ASSERT(a == b, "iget returns same inode");
    CTEST_ASSERT(a->ref_count == 2, "refcount incremented");
}

void test_ipush(void){
    incore_free_all();

    struct inode inode = {0};

    inode.ref_count = 2;
    iput(&inode);

    CTEST_ASSERT(inode.ref_count == 1, "refcount decremented");

    incore_free_all();

    struct inode out = {0};

    out.inode_num = 42;
    out.size = 1234;
    out.ref_count = 1;

    iput(&out);

    struct inode in = {0};
    read_inode(&in, 42);

    CTEST_ASSERT(in.size == 1234, "iput writes inode to disk");
    CTEST_ASSERT(out.ref_count == 0, "refcount reaches zero");
}

void test_mkfs(void){
    mkfs();
    struct inode *root = iget(0);

    CTEST_ASSERT(root != NULL, "root inode exists");
    CTEST_ASSERT(root->flags == 2, "root is directory");
    CTEST_ASSERT(root->size == 64, "root size is accurate");

    iput(root);
}

void test_dir(void){
    mkfs();

    struct directory *dir = directory_open(0);
    struct directory_entry ent;

    CTEST_ASSERT(dir != NULL, "root inode opens");
    CTEST_ASSERT(dir->offset == 0, "offset starts at 0");

    CTEST_ASSERT(directory_get(dir, &ent) == 0, "read first entry");
    CTEST_ASSERT(strcmp(ent.name, ".") == 0, "first entry is .");

    CTEST_ASSERT(dir->offset == DIR_ENTRY_SIZE, "offset advanced");

    CTEST_ASSERT(directory_get(dir, &ent) == 0, "read second entry");
    CTEST_ASSERT(strcmp(ent.name, "..") == 0, "second entry is ..");

    CTEST_ASSERT(directory_get(dir, &ent) == -1, "reaches the end of directory");

    directory_close(dir);
}

int main(void){
    CTEST_VERBOSE(1);

    test_image();
    test_rw();
    test_free();

    test_ialloc();
    test_alloc();

    test_incore();
    test_rw_inode();
    test_iget();
    test_ipush();

    // ls();
    test_mkfs();
    test_dir();

    // Finish Testing
    CTEST_RESULTS();
    CTEST_EXIT();
}