EDITOR=nano

BAUDRATE=115200
SRC=main
AVR_TYPE=atmega8
AVR_DEVICE=m8
AVR_FREQ=16000000UL
PROGRAMMER=usbasp #avr910
DEVICE=/dev/ttyACM0 #COM3
CC = avr-gcc
CFLAGS=-g -DF_CPU=$(AVR_FREQ) -funsigned-char -Os -Werror -Wextra -lm

ifeq ($(OS),Windows_NT)
	# Windows stuff
else
    SUDO=sudo
endif

all: object elf hex flash clean

help:
	@echo 'clean		Delete automatically created files.'

edit:
	$(EDITOR) $(SRC).c

makefile:
	$(EDITOR) makefile

clean:
	rm -f -v $(SRC).elf $(SRC).flash.hex $(SRC).o $(SRC).lst $(SRC).hex

object:
	$(CC) $(CFLAGS) -mmcu=$(AVR_TYPE) -Wa,-ahlmns=$(SRC).lst -c -o $(SRC).o $(SRC).c

elf: object
	$(CC) $(CFLAGS) -mmcu=$(AVR_TYPE) -o $(SRC).elf $(SRC).o

hex: elf
	avr-objcopy -j .text -j .data -O ihex $(SRC).elf $(SRC).hex	

flash: hex
	$(SUDO) avrdude -F -e -b $(BAUDRATE) -c $(PROGRAMMER) -p $(AVR_DEVICE) -P $(DEVICE) -U flash:w:$(SRC).hex
