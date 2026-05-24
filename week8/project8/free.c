#include <unistd.h>
#include "block.h"

#define BITS_PER_BYTE 8

// set a specific bit to the value in set (0 or 1)
void set_free(unsigned char *block, int num, int set){
    int byte_num = num / BITS_PER_BYTE;  // 8 bits per byte
    int bit_num = num % BITS_PER_BYTE;

    int byte = block[byte_num];
    int new_byte = 0b0;

    if(set == 1)
        new_byte = byte | (1 << bit_num);
    
    else if(set == 0) 
        new_byte = byte & ~(1 << bit_num);

    block[byte_num] = new_byte;
}

// Returns index of 0 bit : [76543210]
int find_low_clear_bit(unsigned char byte){
    for (int i = 0; i < 8; i++)
        if (!(byte & (1 << i)))
            return i;
    
    return -1;
}

// find a 0 bit and return its index (i.e. the block number that corresponds to this bit).
int find_free(unsigned char *block){
    for(int i = 0; i < BLOCK_SIZE; i++){
        int bit_index = find_low_clear_bit(block[i]);
        if(bit_index != -1)
            return (i*BITS_PER_BYTE) + bit_index;
    }
    return -1;
}
