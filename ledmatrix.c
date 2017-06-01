#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include<util/delay.h>
#include<stdbool.h>
#include<string.h>

#define R1 PD4
#define R2 PC0
#define R3 PC1
#define R4 PD1
#define R5 PC3
#define R6 PD2
#define R7 PD6
#define R8 PD7

#define C1 PC2
#define C2 PB5
#define C3 PC4
#define C4 PB4
#define C5 PD3
#define C6 PC5
#define C7 PD5
#define C8 PD0

#define DIM 8
void set_pins(){
    DDRD |= (1 << C8);
    DDRD |= (1 << R4);
    DDRD |= (1 << R6);
    DDRD |= (1 << C5);
    DDRD |= (1 << R1);
    DDRD |= (1 << C7);
    DDRD |= (1 << R7);
    DDRD |= (1 << R8);

    DDRC |= (1 << C6);
    DDRC |= (1 << C3);
    DDRC |= (1 << R5);
    DDRC |= (1 << C1);
    DDRC |= (1 << R3);
    DDRC |= (1 << R2);

    DDRB |= (1 << C2);
    DDRB |=(1 << C4);
}

void draw_row(uint8_t row, int index){
}
int main(void){
    set_pins();
    draw_row(0b11111111, 0);

}
