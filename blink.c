#include <avr/io.h>
#include <stdint.h>
#include<util/delay.h>
#include<stdbool.h>

#define n_rows 8
#define n_cols 8

int row_LEDs[n_rows] = {PC0, PC1, PC2, PC3, PC4, PC5, PB1, PB2}; //Fucking lack of pins FUCK
int col_LEDs[n_cols] = {PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7};


void output_low(int port, int pin){
    port &= ~(1 << pin);
}

void output_high(int port, int pin){
    port |= (1 << pin);
}
void setup_pins(){
    DDRD = 0xff;
    DDRC = 0xff;
    DDRB = 0xff;
}

void set_row(int n, bool outputHigh){
    if(n >= 6){
        if(outputHigh){
            PORTB |= (1 << row_LEDs[n]);
        } else{
            PORTB &= ~(1 << row_LEDs[n]);
        }
    }
    else{
        if(outputHigh){
            PORTC |= (1 << row_LEDs[n]);
        } else{
            PORTC &= ~(1 << row_LEDs[n]);
        }
    }
}


void set_col(int n, bool outputHigh){
        if(outputHigh){
            PORTD &= ~(1 << col_LEDs[n]);
        } else{
            PORTD |= (1 << col_LEDs[n]);
        }
}
void light_led_faster(int x, int y){
    PORTC &= (1 << y);

}
void light_led(int x, int y){
    if(y > 5){
        PORTC = 0;
        PORTB = (1<<row_LEDs[y]);
    } else {
        PORTB = 0;
        PORTC = (1<<row_LEDs[y]);
    }
    PORTD = ~(1 << col_LEDs[x]);
}
void col_draw(int n, uint8_t col){
    if(n > 5){
        PORTC = 0;
        PORTB = (1<<row_LEDs[n]);
    } else {
        PORTB = 0;
        PORTC = (1<<row_LEDs[n]);
    }
    PORTD  = col;

}
int main(void){
    setup_pins();
    while(1){
        for(int i = 0; i < n_rows; i++){
            for(int j = 0; j < n_cols; j++){
                light_led(i, j);
            }
        }
    }
}
