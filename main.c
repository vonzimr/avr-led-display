#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include<util/delay.h>
#include<stdbool.h>
#include<string.h>
#include "conway.h"
#include "display.h"

int main(void){
    set_pins();
    disp_t board;
    while(1){
        draw(board);
        new_generation(board);
    }
}
