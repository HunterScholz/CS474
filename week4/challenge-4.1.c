#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int *global;

int main(void){

    int *local;
    int *heap = (int*)malloc(sizeof(int));

    printf("Local: %p\n", (void*)&local);
    printf("Global: %p\n", (void*)&global);
    printf("Heap: %p\n", heap);
    printf("Main: %p\n", (void*)&main);
}