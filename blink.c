#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include<util/delay.h>
#include<stdbool.h>
#include<string.h>
#define n_rows 8
#define n_cols 8
#define MAX_STAGNANT 5
int stagnant_generations = 0;

enum PORTS {D, C, B};
int row_LEDs[n_rows][2] = {{C, PC0}, 
                           {C, PC1}, 
                           {C, PC2}, 
                           {C, PC3}, 
                           {C, PC4}, 
                           {C, PC5}, 
                           {B, PB1}, 
                           {B, PB2}};
int col_LEDs[n_cols][2] = {{D,PD0}, {D,PD1}, {D,PD2}, {D,PD3}, {D,PD4}, {D,PD5}, {D,PD6}, {D,PD7}};


void setup_pins(int pin_list[8][2], int len){
    int port;
    for(int i = 0; i < len; i++){
        port = pin_list[i][0];

        switch(port){
        case D: 
            DDRD |= (1 << pin_list[i][1]);
        case C: 
            DDRC |= (1 << pin_list[i][1]);
        case B: 
            DDRB |= (1 << pin_list[i][1]);
        break;
        }
    }
}


void draw(uint8_t* board){
    int port;
    for(int i = 0; i < n_rows; i++){
        port = row_LEDs[i][0];
        switch(port){
            case C: {
                        PORTB = 0;
                        PORTC = (1 << row_LEDs[i][1]);
                        break;}
            case B: 
                    {
                        PORTC = 0;
                        PORTB = (1 << row_LEDs[i][1]);
                        break;
                    }
        }
        PORTD = ~board[i];
        _delay_ms(1); //without this the LEDs will ghost
    }

}

void col_draw(int n, uint8_t col){

    if(n > 5){
        PORTC = 0;
        PORTB = (1<<row_LEDs[n][1]);
    } else {
        PORTB = 0;
        PORTC = (1<<row_LEDs[n][1]);
    }
    PORTD  = ~col;

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
    int shift = 6-x;
    int mask = 0;
    if(shift<=0){
        mask = 0b11; 
    }
    else {
        mask = 0b111 << shift;
    }
    return  bit_count(mask & row); 
}

int get_neighbors(uint8_t* board, int x, int y){
    int num_neighbors = 0;

    num_neighbors += get_row_count(board[y], x);

    if(y-1 > 0){
        num_neighbors += get_row_count(board[y-1], x);
    }

    if(y+1 < 8){
        num_neighbors += get_row_count(board[y+1], x);
    }
    return num_neighbors - get_cell(board, x, y);


}

void cell_on(uint8_t* board, int x, int y){
    uint8_t cell_mask = (1 << ((n_cols - 1) - x));
    board[y] |= cell_mask;
}

void cell_off(uint8_t* board, int x, int y){

    uint8_t cell_mask = ~(1 << ((n_cols- 1) - x));
    board[y] &= cell_mask;

}
void diff_board(uint8_t* a, uint8_t* b){
    int diff = 0;
    for(int i = 0; i < n_rows; i++){
        diff += a[i] - b[i];

    }
    if(diff==0){
        stagnant_generations++;
    }
}
void randomize_board(uint8_t* board){
    int shiftiness = 2;
    for(int i = 0; i < 8; i++){
        board[i] = rand() % 255;
        board[i] = board[i] << rand() % shiftiness;

    }

    stagnant_generations = 0;

}
void updateCells(uint8_t *board){
    uint8_t old_board[8];
    memcpy(old_board, board, sizeof(old_board));
    int neighbors = 1;
    for(int i = 0; i < n_rows; i++){
        for(int j = 1; j < n_cols-1; j++){
            neighbors = get_neighbors(old_board, i, j);
            if(neighbors < 2 || neighbors > 3){
                cell_off(board, i, j);
            }
            if(neighbors == 3){
                cell_on(board, i, j);
            }
            draw(old_board);
        }
    }
    diff_board(old_board, board);

    if(stagnant_generations > MAX_STAGNANT){
        randomize_board(board);

    }
}
int main(void){
    setup_pins(row_LEDs, n_rows);
    setup_pins(col_LEDs, n_cols);
    uint8_t board[8] = {0b00000000, 
                        0b00100000, 
                        0b00101000, 
                        0b00110000, 
                        0b00000000, 
                        0b00000000, 
                        0b00000000, 
                        0b00000000};


    int cur_cell = 0;
    cell_on(board, 2, 2);
    int count = 0;
    while(1){
        if(count == 10){
            updateCells(board);
            count = 0;
        }
        draw(board);
        count++;
    }
}
