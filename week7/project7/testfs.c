#include "image.h"
#include "block.h"
#include "free.h"
#include "inode.h"

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

    CTEST_ASSERT(ialloc() == 0, "ialloc inode 0");
    CTEST_ASSERT(ialloc() == 1, "ialloc inode 1");
    CTEST_ASSERT(ialloc() == 2, "ialloc inode 2");

    // Fill block with 1s
    unsigned char buf[BLOCK_SIZE];
    memset(buf, 0xFF, BLOCK_SIZE);
    bwrite(INODE_MAP_BLOCK, buf);

    CTEST_ASSERT(ialloc() == -1, "ialloc fails when full");
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

int main(void){
    CTEST_VERBOSE(1);

    test_image();
    test_rw();
    test_free();
    test_ialloc();
    test_alloc();


    // Finish Testing
    CTEST_RESULTS();
    CTEST_EXIT();
}