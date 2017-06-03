#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include<util/delay.h>
#include<stdbool.h>
#include<string.h>

#include "conway.h"
#include "display.h"

int main(void){
    disp_t board = disp_new();
    while(1){
        new_generation(board);
    }
}
