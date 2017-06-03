#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdbool.h>
#include "display.h"


//map value of bit a to index b
uint8_t col_b_map[8] = {0, 0, 0, 0, 2, 4, 0, 0};
uint8_t col_c_map[8] = {0, 0, 1, 0, 3, 6, 0, 0};
uint8_t row_c_map[8] = {2, 3, 0, 5, 0, 0, 0, 0};
uint8_t col_d_map[8] = {7, 0, 5, 0, 0, 8, 0, 0};
uint8_t row_d_map[8] = {0, 1, 0, 6, 4, 0, 8, 7};

uint8_t map_bit(uint8_t bits, int to, int from){
    if(from != 0){
        uint8_t bit = ((1 << (from-1)) & bits) >> (from-1);
        return bit << to;
    }
    else{
        return 0;
    }
}
void draw_row(uint8_t col, int index){
    uint8_t portb = 0;
    uint8_t portc = 0;
    uint8_t portd = 0;
    uint8_t row = 0b1 << index;
    col = ~col;

    
    for(int i = 0; i < 8; i++){
        portb |= map_bit(col, i, col_b_map[i]);
        portc |= map_bit(col, i, col_c_map[i]);
        portd |= map_bit(col, i, col_d_map[i]);

        portc |= map_bit(row, i, row_c_map[i]);
        portd |= map_bit(row, i, row_d_map[i]);
    }

    PORTB = portb;
    PORTD = portd;
    PORTC = portc;

}

void draw(disp_t board){
    
    for(int i= 0 ; i < DIM; i++){
        draw_row(board[i], i);

    }
}


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

    //INPUT
    DDRB &= ~(1 << PB1);
    DDRB &= ~(1 << PB2);

}

int get_cell(uint8_t* board, int x, int y){
    return (board[y] >> ((8 - 1) -  x)) & 0x1;
}
int bit_count(uint8_t bits){

    bits = (bits & 0b01010101) + ((bits & 0b10101010) >> 1);
    bits = (bits & 0b00110011) + ((bits & 0b11001100) >> 2); 

    return (bits & 0b00001111) + ((bits & 0b11110000) >> 4);



}
int get_row_count(uint8_t row, int x){
    uint8_t pos = 0b10000000 >> x;
    uint8_t neighbors = (pos >> 1) | (pos << 1) | pos;
    return  bit_count(neighbors & row); 
}

void cell_on(disp_t board, int x, int y){
    uint8_t cell_mask = (1 << ((DIM - 1) - x));
    board[y] |= cell_mask;
}

void cell_toggle(disp_t board, int x, int y){
    uint8_t cell_mask = (1 << ((DIM - 1) - x));
    board[y] ^= cell_mask;
}
void cell_off(disp_t board, int x, int y){

    uint8_t cell_mask = ~(1 << ((DIM - 1) - x));
    board[y] &= cell_mask;

}
int diff_board(disp_t a, disp_t b){
    int diff = 0;
    for(int i = 0; i < DIM; i++){
        diff += bit_count(a[i]) - bit_count(b[i]);

    }
    return diff;
}

disp_t disp_new(){
    set_pins();
    disp_t disp = (disp_t) malloc(8*sizeof(disp_t));
    return disp;

}

void disp_free(disp_t disp){
    free(disp);
}

void delay_draw(disp_t disp, int delay){
    for(int i = 0; i < delay; i++){
        draw(disp);
    }
}

void screen_row_wipe(disp_t disp){
    for(int i = 0; i < 8; i++){
            disp[i] = disp[i] ^ 0xff;
            delay_draw(disp, 100);
        }
}

void screen_dissolve(disp_t disp){
    uint8_t dissolved = 0;
    int next_row;
    while(dissolved != 0xff){
        next_row = rand() % 8;
        disp[next_row] |= rand() % 256;
        if(disp[next_row] == 0xff){
            dissolved |= 1 << next_row;
        }
        delay_draw(disp, 20);
    }
}

void screen_cell_wipe(disp_t disp){
    for(int i = 0; i < DIM; i++){
        for(int j = 0; j < DIM; j++){
            cell_toggle(disp, i, j);
            delay_draw(disp, 10);
        }
    }
}

void screen_random_transition(disp_t disp){
    int choice = rand() % 3;
    switch(choice){
        case 0:
            screen_row_wipe(disp);
            break;
        case 1:
            screen_dissolve(disp);
            break;
        case 2:
            screen_cell_wipe(disp);
            break;
        }
}
