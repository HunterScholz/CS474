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

/*
Local: 0x7fffc20e6c48
Global: 0x57467ab2c018
Heap: 0x5746afe6f2a0
Main: 0x57467ab29189
*/