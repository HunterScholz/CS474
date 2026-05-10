#include "image.h"
#include "block.h"
#include "ctest.h"
#include <string.h>

int main(void){
    CTEST_VERBOSE(1);

    // Test Image
    CTEST_ASSERT(image_open("foo.img", 1) != -1, "test image open");
    CTEST_ASSERT(image_close() == 0, "test image close");

    // Test Read & Write
    image_open("foo.img", 1);

    unsigned char read_buf[BLOCK_SIZE] = {0};
    unsigned char write_buf[BLOCK_SIZE] = {0};

    strcpy((char *)write_buf, "hello, world");
    bwrite(3, write_buf);

    CTEST_ASSERT(
        strcmp((const char *)bread(3, read_buf), "hello, world") == 0, 
        "test writing to a block and reading"
    );

    // Finish Testing
    CTEST_RESULTS();
    CTEST_EXIT();
}