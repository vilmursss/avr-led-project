# Directories
SRC_DIR := .
BUILD_DIR := $(SRC_DIR)/build
INC_DIRS := $(SRC_DIR) \
			$(SRC_DIR)/utils \
			$(SRC_DIR)/register \
			$(SRC_DIR)/led \
			$(SRC_DIR)/pwm \

# Source and Object Files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c) \
	$(wildcard $(SRC_DIR)/utils/**/*.c) \
	$(wildcard $(SRC_DIR)/register/**/*.c) \
	$(wildcard $(SRC_DIR)/serial/*.c) \
	$(wildcard $(SRC_DIR)/led/*.c) \
	$(wildcard $(SRC_DIR)/pwm/*.c) \

OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Compiler and Flags
CC := avr-gcc
CFLAGS := -mmcu=atmega328p -DF_CPU=16000000UL -DSERIAL_BAUD=9600 $(foreach dir, $(INC_DIRS), -I$(dir)) -Wall -Os -ffunction-sections -fdata-sections
LDFLAGS := -Wl,--gc-sections

# Define output and avrdude flags
TARGET := $(BUILD_DIR)/avr-led
AVRDUDE_FLAGS := -c arduino -p atmega328p -P /dev/ttyS0 -b 115200 -D

# Default target
all: $(TARGET).hex

# Compile and link
$(TARGET).elf: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Convert ELF to HEX
$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -R .eeprom $< $@

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/utils/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Upload the HEX file to the target device
upload: $(TARGET).hex
	avrdude $(AVRDUDE_FLAGS) -U flash:w:$<:i

# Clean up build files
clean:
	rm -r $(BUILD_DIR)

size: $(TARGET).elf
	avr-size --format=avr --mcu=atmega328p $<

.PHONY: all upload clean print-vars
