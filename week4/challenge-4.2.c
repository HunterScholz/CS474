#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int grow_stack(int n){
    if(n == 0){
        return 0;
    }

    int stack = n;
    printf("Stack: %p\n", (void*)&stack);

    return grow_stack(n-1);
}


int main(void){

    // Grow Heap
    for(int i = 0; i < 5; i++){
        int *heap = (int*)malloc(sizeof(int));
        printf("Heap: %p\n", heap);
    }

    // Grow Stack
    grow_stack(5);
}


/*
Heap: 0x5d162e3ff2a0
Heap: 0x5d162e3ff6d0
Heap: 0x5d162e3ff6f0
Heap: 0x5d162e3ff710
Heap: 0x5d162e3ff730
Stack: 0x7ffc3790d8e4
Stack: 0x7ffc3790d8b4
Stack: 0x7ffc3790d884
Stack: 0x7ffc3790d854
Stack: 0x7ffc3790d824
*/