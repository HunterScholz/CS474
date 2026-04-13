#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

const int SIZE=1024;  // bytes

struct firework {
    int shell_caliber;
    float lift_charge;
    float burst_charge;
    int fuse_delay_ms;
};

int main(void)
{
    // Allocate some space from the OS
    void *heap = mmap(NULL, SIZE, PROT_READ|PROT_WRITE,
                      MAP_ANON|MAP_PRIVATE, -1, 0);

    // Challenge: Store your favorite letter at the byte `heap` points
    // to, then print it with `%c` and also with `%d`.
    
    char *p = heap;
    p = "S";
    printf("My favorite letter is %c\n", *p);
    printf("My favorite letter is %d\n", *p);

    // Challenge: Store a string at the byte `heap` points to, then
    // print it.
    
    char *s = heap;
    strcpy(s, "This is a string");
    printf("%s\n", s);

    // Challenge: Store an integer at the byte `heap` points to, then
    // print it.
    
    int *i = heap;
    *i = 10;
    printf("int: %d ", *i);

    // Challenge: Store a float right *after* the int you just stored.
    // Print them both.
    
    float *f = heap + sizeof(int);
    *f = 5.5;
    printf("float: %f\n", *f);

    // Challenge: Store a short (a smallish integer) at byte offset 512
    // in the `heap` area. Then print it with `%d`, along with the int
    // and float you already stored.
    
    int *si = heap + 512;
    *si = 3;
    printf("i: %d, f: %f, si: %d\n", *i, *f, *si);

    // Challenge: Store a struct firework at byte offset 48 in the
    // `heap` area. Make a `struct firework *` initialized to the right
    // position, then fill it with data (with the `->` operator). Then
    // print out the data, and also print out the int, float, and short
    // from above.
    
    struct firework *fw = heap + 48;

    fw->shell_caliber = 10;
    fw->lift_charge = 1.5;
    fw->burst_charge = 2.0;
    fw->fuse_delay_ms = 500;

    printf("%d, %f, %d, %d, %f, %f, %d\n", *i, *f, *si, fw->shell_caliber,
                                                        fw->lift_charge,
                                                        fw->burst_charge,
                                                        fw->fuse_delay_ms);

    // Challenge: Store a second struct firework directly after the
    // first one. Initialize and print.

    struct firework *fw2 = heap + 48 + sizeof(struct firework);

    fw2->shell_caliber = 15;
    fw2->lift_charge = 1.0;
    fw2->burst_charge = 3.0;
    fw2->fuse_delay_ms = 300;

    printf("%d, %f, %f, %d\n", fw2->shell_caliber,
                               fw2->lift_charge,
                               fw2->burst_charge,
                               fw2->fuse_delay_ms);
}
