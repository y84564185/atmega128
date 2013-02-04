#makefile for mega128
CC=avr-gcc
OBJCOPY=avr-objcopy
MCU=atmega128
SMPMCU=m128
SIZE=avr-size
#our obj file name
OBJNAME=obj
#our source file list
SOURCEFILE=i2c.c adxl345.c uart.c

all:$(OBJNAME).hex

$(OBJNAME).o:$(SOURCEFILE)
	$(CC) -mmcu=$(MCU) -Wall -Os -o $(OBJNAME).o $(SOURCEFILE)
$(OBJNAME).hex:$(OBJNAME).o
	$(OBJCOPY) -j .text -j .data -O ihex $(OBJNAME).o $(OBJNAME).hex
	$(SIZE) $(OBJNAME).o
	$(SIZE) $(OBJNAME).hex

.PHONY:download
download:
	avrdude -p $(SMPMCU) -c usbasp -e -U flash:w:$(OBJNAME).hex
.PHONY:clean
clean:
	rm -f $(OBJNAME).o $(OBJNAME).hex
