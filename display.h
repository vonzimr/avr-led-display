#include<stdint.h>

#ifndef DISPLAY_H
#define DISPLAY_H

#define R4 PD1
#define R6 PD2
#define R1 PD4
#define R7 PD6
#define R8 PD7

#define R2 PC0
#define R3 PC1
#define R5 PC3

#define C1 PC2
#define C3 PC4
#define C6 PC5

#define C2 PB5
#define C4 PB4

#define C8 PD0
#define C5 PD3
#define C7 PD5

#define DIM 8
#define n_rows 8
#define n_cols 8
#define MAX_STAGNANT

typedef uint8_t* disp_t;

void draw_row(uint8_t col, int index);

void draw(disp_t board);

int get_cell(disp_t board, int x, int y);

int get_row_count(uint8_t row, int x);

void cell_on(disp_t board, int x, int y);


void cell_off(disp_t board, int x, int y);

int diff_board(disp_t a, disp_t b);

disp_t disp_new();
void disp_free(disp_t disp);

void screen_random_transition(disp_t disp);
void screen_row_wipe(disp_t disp);
void screen_cell_wipe(disp_t disp);
void screen_dissolve(disp_t disp);

void delay_draw(disp_t board, int delay);

#endif
