#include <avr/io.h>
#include <stdint.h>
#include<util/delay.h>
#include<stdbool.h>
#include<string.h>
#define n_rows 8
#define n_cols 8

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

//Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
//Any live cell with two or three live neighbours lives on to the next generation.
//Any live cell with more than three live neighbours dies, as if by overpopulation.
//Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

int get_cell(uint8_t* board, int x, int y){
    return (board[y] >> ((n_cols - 1) -  x)) & 0x1;
}

int get_neighbors(uint8_t* board, int i, int j){
    int num_neighbors = 0;
    num_neighbors += get_cell(board, i-1, j-1);
    num_neighbors += get_cell(board, i  , j-1);
    num_neighbors += get_cell(board, i+1, j-1);

    num_neighbors += get_cell(board, i-1, j+1);
    num_neighbors += get_cell(board, i  , j+1);
    num_neighbors += get_cell(board, i+1, j+1);

    num_neighbors += get_cell(board, i-1, j);
    num_neighbors += get_cell(board, i+1, j);
    return num_neighbors;
}

void cell_on(uint8_t* board, int x, int y){
    uint8_t cell_mask = (1 << ((n_cols - 1) - x));
    board[y] |= cell_mask;
}

void cell_off(uint8_t* board, int x, int y){

    uint8_t cell_mask = ~(1 << ((n_cols- 1) - x));
    board[y] &= cell_mask;

}
void updateCells(uint8_t *board){
    uint8_t old_board[8];
    memcpy(old_board, board, sizeof(old_board));
    int neighbors = 0;
    for(int i = 1; i < n_rows-1; i++){
        for(int j = 1; j < n_cols-1; j++){
            neighbors = get_neighbors(old_board, i, j);
            if(neighbors < 2 || neighbors > 3){
                cell_off(board, i, j);
            }
            if(neighbors == 3){
                cell_on(board, i, j);
            }
        }
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
        if(count == 1000){
            updateCells(board);
            count = 0;
        }
        draw(board);
        count++;
    }
}
