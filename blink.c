#include <avr/io.h>
#include <stdint.h>
#include<util/delay.h>

int main(void){
    DDRD = 0xff; 
    PORTD = 0xff;
    _delay_ms(200);
    while(1){
        PORTD = PORTD << 1;
        _delay_ms(200);
        if(!PORTD){
            PORTD = 0xff;
            _delay_ms(200);
        }
    }

}
