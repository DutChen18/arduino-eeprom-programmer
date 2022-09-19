CC	= avr-gcc
MCU	= atmega328p
CFLAGS	= -mmcu=$(MCU) -Os
OBJECTS	= main.o

all: a.hex
	avrdude -p $(MCU) -U flash:w:a.hex -c arduino -P /dev/ttyUSB0

a.hex: a.out
	avr-objcopy -R .eeprom -O ihex $< $@

a.out: $(OBJECTS)
	$(CC) $(CFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@ -c
