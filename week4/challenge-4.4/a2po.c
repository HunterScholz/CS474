#include <stdio.h>
#include <stdlib.h>

void usage(){
    printf("Usage: ./a2po int\n");
    exit(1);
}


int main(int argc, char *argv[]){
    
    if(argc != 2){
        usage();
    }

    unsigned int off;

    int address = atoi(argv[1]);

    int pg = address >> 8;
    off = (1 << 8) -1;
    off = address & off;

    printf("Address %d = Page %d, Offset %d\n", address, pg, off);
}