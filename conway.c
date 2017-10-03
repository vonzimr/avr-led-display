#include <stdint.h>
#include <stdlib.h>
#include<string.h>

#include "display.h"

int stagnant_generations = 0; 

int get_neighbors(disp_t board, int x, int y){
    int num_neighbors = 0;

    num_neighbors += get_row_count(board[y], x);

    if(y-1 >= 0){
        num_neighbors += get_row_count(board[y-1], x);
    }

    if(y+1 <= 7){
        num_neighbors += get_row_count(board[y+1], x);
    }
    return num_neighbors - get_cell(board, x, y);


}

void randomize_board(disp_t board){
    int shiftiness = 2;
    for(int i = 0; i < 8; i++){
        board[i] = rand() % 255;
        board[i] = board[i] << rand() % shiftiness;

    }

    stagnant_generations = 0;

}

void new_generation(disp_t board){
    disp_t old_board;

    memcpy(old_board, board, sizeof(disp_t));
    int neighbors = 0;
    for(int i = 0; i < DIM; i++){
        for(int j = 0; j < DIM; j++){
            neighbors = get_neighbors(old_board, i, j);
            if(neighbors < 2 || neighbors > 3){
                cell_off(board, i, j);
            }
            if(neighbors == 3){
                cell_on(board, i, j);
            }
        }

    }
    if(diff_board(old_board, board) < 5){
      stagnant_generations++;
    };

    if(stagnant_generations > 8){
        delay_draw(board, 800);
        screen_random_transition(board);
        delay_draw(board, 800);
        randomize_board(board);
    }
    delay_draw(old_board,100);
}
