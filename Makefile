MCU=attiny45
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-g -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues
PROG=avrdude
INTERFACE=usbasp

#-------------------
all: main.hex

main.hex : main.out 
	$(OBJCOPY) -R .eeprom -O ihex main.out main.hex 

main.out : main.o DHT.o
	$(CC) $(CFLAGS) -o main.out -Wl,-Map,main.map main.o DHT.o

main.o : main.c DHT.h
	$(CC) $(CFLAGS) -Os -c main.c
	
DHT.o : DHT.c DHT.h
	$(CC) $(CFLAGS) -Os -c DHT.c
		
#-------------------
help: 
	@echo "Type make or make install"	
#-------------------
install:
	$(PROG) -p $(MCU) -c $(INTERFACE) -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m -U flash:w:main.hex	
clean:
	rm -f *.o *.map *.out main.hex
#-------------------
