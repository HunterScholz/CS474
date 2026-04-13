#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

const int SIZE=1024;  // bytes

struct bicycle {
    int wheel_count;
    char name[32];
};

int main(void)
{
    // Allocate some space from the OS
    void *heap = mmap(NULL, SIZE, PROT_READ|PROT_WRITE,
                  MAP_ANON|MAP_PRIVATE, -1, 0);

    // Challenge 1:
    //
    // Treat heap as an array of chars
    // Store a string there with strcpy() and print it

    strcpy(heap, "Hello, World!");
    printf("%s\n", (char *)heap);

    // Challenge 2:
    //
    // Treat heap as an array of ints
    // Use a loop to store the values 0, 10, 20, 30, 40 in it
    // Use a loop to retrieve the value and print them

    for(int i = 0; i < 5; i++){
        int *p = heap + i * sizeof(int);
        *p = i*10;
    }

    for(int i = 0; i < 5; i++)
        printf("%d ", *(int *)(heap + i * sizeof(int)));
    printf("\n");

    // Challenge 3:
    //
    // Treat heap as an array of struct bicycles
    // Store 3 bicycles
    // Print out the bicycle data in a loop

    struct bicycle *b = (struct bicycle *)heap;

    for(int i = 0; i < 3; i++){
        sprintf((b+i)->name, "Bike %d", i);
        (b+i)->wheel_count = i;
    }

    for(int i = 0; i < 3; i++)
        printf("%s: %d Wheels\n", (b+i)->name, (b+i)->wheel_count);

    // Challenge 4:
    //
    // If you used pointer notation, redo challenges 1-3 with array
    // notation, and vice versa.

    // C1: Unchanged
    strcpy(heap, "Hello, World!");
    printf("%s\n", (char *)heap);

    // C2:
    int *p = (int *)heap;

    for(int i = 0; i < 5; i++)
        p[i] = i*10;

    for(int i = 0; i < 5; i++)
        printf("%d ", p[i]);
    printf("\n");

    // C3:
    struct bicycle *bc = (struct bicycle *)heap;

    for(int i = 0; i < 3; i++){
        sprintf(bc[i].name, "Bike %d", i);
        bc[i].wheel_count = i;
    }

    for(int i = 0; i < 3; i++)
        printf("%s: %d Wheels\n", bc[i].name, bc[i].wheel_count);

    // Challenge 5:
    //
    // Make the first 32 bytes of the heap a string (array of chars),
    // and follow that directly with an array of ints.
    //
    // Store a short string in the string area of the heap
    // Use a loop to store the values 0, 10, 20, 30, 40 in the int array
    // Use a loop to retrieve the value and print them
    // Print the string

    strcpy(heap, "Hello, World!");

    for(int i = 0; i < 5; i++){
        int *p = heap + 32 + i * sizeof(int);
        *p = i*10;
    }

    for(int i = 0; i < 5; i++)
        printf("%d ", *(int *)(heap + 32 + i * sizeof(int)));
    printf("\n");
    printf("%s\n", (char *)heap);

    // Challenge 6:
    //
    // Make an array of interleaved `struct bicycle` and `int` data
    // types.
    //
    // The first entry in the heap will be a `struct bicycle`, then
    // right after that, an `int`, then right after that a `struct
    // bicycle`, then an `int`, etc.
    //
    // Store as many of these as can fit in SIZE bytes. Loop through,
    // filling them up with programmatically-generated data. Then loop
    // through again and print out the elements.

    int i = 0;
    int filled = 0;
    int total_size = sizeof(struct bicycle) + sizeof(int);

    while(filled < SIZE){
        // Add Bicylce
        struct bicycle *b = (struct bicycle *)(heap + filled);
        sprintf((b)->name, "Bike %d", i);
        (b)->wheel_count = i;

        // Add Int
        int *n = (int *)(heap + filled + sizeof(struct bicycle));
        *n = i;
        
        i++;
        filled = i * total_size;
    }

    for(int i = 0; i < SIZE / total_size; i++){
        void *pointer = heap + i * total_size;

        struct bicycle *b = (struct bicycle *)pointer;

        int *n = (int *)(pointer + sizeof(struct bicycle));

        printf("%d | %s: %d wheels\n", *n, b->name, b->wheel_count);
    }

    // Free it up
    munmap(heap, SIZE);
}
