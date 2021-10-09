MCU=attiny85
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-g -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues
PROG=avrdude
INTERFACE=usbasp

#-------------------
all: main.hex

main.hex : main.out 
	$(OBJCOPY) -R .eeprom -O ihex main.out main.hex 

main.out : main.o
	$(CC) $(CFLAGS) -o main.out -Wl,-Map,main.map main.o

main.o : main.c
	$(CC) $(CFLAGS) -Os -c main.c
		
#-------------------
help: 
	@echo "Type make or make install"	
#-------------------
install:
	$(PROG) -p $(MCU) -c $(INTERFACE) -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m -U flash:w:main.hex	
clean:
	rm -f *.o *.map *.out main.hex
#-------------------
