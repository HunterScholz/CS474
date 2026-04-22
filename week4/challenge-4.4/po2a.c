#include <stdio.h>
#include <stdlib.h>

void usage(){
    printf("Usage: ./po2a int int\n");
    exit(1);
}


int main(int argc, char *argv[]){
    
    if(argc != 3){
        usage();
    }

    int pg = atoi(argv[1]);
    int off = atoi(argv[2]);

    int address = (1 << 8) -1;
    address = address << 8;

    int high_8 = pg << 8;
    address = high_8 & address;
    address = off | address;

    printf("Page %d, Offset %d = Address %d\n", pg, off, address);
}