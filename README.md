# avr-led-display
Conway's Game of Life on an 8x8 LED Matrix

## What works:
  - Transitions effects:
      - left / right / up /down swipe effect
      - random dissolve
  - Automatic generation of new conway board
    - It is fairly hard to predict cycles on a conway board, currently the program looks for differences between a few
      generations of the game, if the differences are below a certain threshold the game restarts and applies a specific transition.
  - Function which maps LED matrix's row/column pins to the pins located on an arduino
    - These can be changed to best suit your application or led display. I've found the pinouts of led matrices
      are not particularly well standardized, and certain applications may require different pin layouts. These can
      be easily modified.
      
## What still needs to be done:
 - more general screen rendering. A conway board is nice, but I'd also like access to, for instsance, custom text that could
   be displayed. Work has been started in this direction, and I've found a few 8x8 fonts that would translate well.
 - Serial input. Being able to display text that is sent over serial would be interesting, and provide interesting new
   uses for the display.
 - Larger displays. being able to daisy chain multiple 8x8 displays would be another interesting feature which would make
   things like text display more usable.
