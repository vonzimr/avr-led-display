DEVICE     = atmega328p
CLOCK      = 2000000
PROGRAMMER = avrispmkii 
BAUD       = 19200
BIT        = 100
FILENAME   = main
COMPILE    = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all: clean main upload

main: conway.o conway.h display.o display.h fonts.o fonts.h
	$(COMPILE) -c $(FILENAME).c -o $(FILENAME).o
	$(COMPILE)  $(FILENAME).o conway.o display.o -o $(FILENAME).elf
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

conway.o: conway.c conway.h
	$(COMPILE) -c conway.c

fonts.o: fonts.c fonts.h
	$(COMPILE) -c fonts.c

display.o: display.c conway.h

	$(COMPILE) -c display.c

upload:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -B$(BIT) -b$(BAUD) -U flash:w:$(FILENAME).hex:i 

fuses:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -B$(BIT) -b$(BAUD) -U lfuse:w:0xff:m -U hfuse:w:0xdf:m -U efuse:w:0xf8:m -U lock:w:0xFF:m

build-debug:
	$(COMPILE) -ggdb -c $(FILENAME).c -o $(FILENAME).o
	$(COMPILE) -o $(FILENAME).elf $(FILENAME).o
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

sim: main 
	simavr -v -mcu $(DEVICE) -freq $(CLOCK) $(FILENAME).hex

clean:
	-rm *.o *.swp *.elf *.hex  2>/dev/null || true
