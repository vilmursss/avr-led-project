# Target microcontroller
MCU = atmega328p

# Clock frequency
F_CPU = 16000000UL

# Define the programmer and port
PROGRAMMER = arduino

# Verify your port as this Makefile was created to work within VirtualBox image
# where serial port is configured from host machine
PORT = /dev/ttyS0

# Define the baud rate for the programmer
BAUD = 115200

# Define serial debug baud rate.
SERIAL_DEBUG_BAUD = 9600

# Find all .c files in the current directory and subdirectories (lazy)
SRC = $(shell find . -name "*.c")

# Include directories
INC_DIR := utils

# Generate object files from sources
OBJ = $(SRC:.c=.o)

# Define the compiler and flags
CC = avr-gcc
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DSERIAL_BAUD=$(SERIAL_DEBUG_BAUD) -I$(INC_DIR) -Wall -Os

# Define the output file
TARGET = avr-led

# Define the avrdude command
AVRDUDE = avrdude
AVRDUDE_FLAGS = -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -D

# Default target
all: $(TARGET).hex

# Compile and link
$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Convert ELF to HEX
$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -R .eeprom $< $@

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Upload the HEX file to the target device
upload: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$<:i

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET).elf $(TARGET).hex

.PHONY: all upload clean
