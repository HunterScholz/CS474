#include "adder.h"
#include <stdio.h>

int main(void){
    int result = adder(2, 4);  // should give 6
    printf("Result: 2 + 4 = %d\n", result);
}