DEVICE     = atmega328p
CLOCK      = 2000000
PROGRAMMER = avrispmkii 
BAUD       = 19200
BIT        = 100
FILENAME   = blink
COMPILE    = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all: clean build upload

build:
	$(COMPILE) -c $(FILENAME).c -o $(FILENAME).o
	$(COMPILE) -o $(FILENAME).elf $(FILENAME).o
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

upload:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -B$(BIT) -b$(BAUD) -U flash:w:$(FILENAME).hex:i 


build-debug:
	$(COMPILE) -ggdb -c $(FILENAME).c -o $(FILENAME).o
	$(COMPILE) -o $(FILENAME).elf $(FILENAME).o
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

sim: build-debug
	simavr -v -g -mcu $(DEVICE) -freq $(CLOCK) $(FILENAME).hex

clean:
	rm blink.o
	rm blink.elf
	rm blink.hex
