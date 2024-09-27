#include "system_clock.h"
#include "system_clock_defs.h"

#include <interrupt/reg_interrupt.h>

#include <avr/interrupt.h>

#include <stdio.h>

// Define the clock frequency if not already defined
#ifndef F_CPU
    #define F_CPU 16000000UL // 16 MHz
#endif

// Constants
const char* TS_FORMAT = "%02d:%02d:%02d";
const uint8_t DESIRED_FREQUENCY = 1; // 1 Hz
const uint32_t PRESCALER = 1024;

// Registers
volatile uint8_t* const TIMSK1_REG = (volatile uint8_t*)0x6F; // Interrupt Flag Register
volatile uint8_t* const TCCR1B_REG = (volatile uint8_t*)0x81; // Timer/Counter1 Control Register B
volatile uint16_t* const ORC1A_REG = (volatile uint16_t*)0x88; //  Timer/Counter1 - Combinced output compare register A for low & high byte

// Local variables
static volatile uint8_t hours = 0;
static volatile uint8_t minutes = 0;
static volatile uint8_t seconds = 0;

// Local functions
static void set_ctc_mode_on();
static void set_clock_select_bits();
static void set_ocr1a_compare_value();
static void enable_compare_a_match_interrupt();

ISR(TIMER1_COMPA_vect)
{
    seconds++;
    if (seconds >= 60)
    {
        seconds = 0;
        minutes++;
    }
    
    if (minutes >= 60)
    {
        minutes = 0;
        hours++;
    }
    
    if (hours >= 24)
    {
        hours = 0;
        printf("Reseting timer... Device has been up and running over 24h\n");
    }
}

void system_clock_init()
{
    set_ctc_mode_on();
    set_clock_select_bits();
    set_ocr1a_compare_value();
    enable_compare_a_match_interrupt();
    enable_interrupts();
}

const char* system_clock_get_ts()
{
    static char time_str[10];
    snprintf(time_str, sizeof(time_str), TS_FORMAT, hours, minutes, seconds);
    
    return time_str;
}

static void set_ctc_mode_on()
{
    // Read the current value of the register
    uint8_t currentValue = *(volatile uint8_t*)TCCR1B_REG;

    // Create a bit field structure and assign the current value to it
    TCCR1Bits* tccr1bBits = (TCCR1Bits*)&currentValue;

    // Set the waveform generation mode to 0b01 (CTC mode)
    tccr1bBits->waveformGenMode = 0x1;

    // Write the modified value back to the register
    *(volatile uint8_t*)TCCR1B_REG = currentValue;
}

static void set_clock_select_bits()
{
    // Read the current value of the register
    uint8_t currentValue = *(volatile uint8_t*)TCCR1B_REG;

    // Create a bit field structure and assign the current value to it
    TCCR1Bits* tccr1bBits = (TCCR1Bits*)&currentValue;

    // Set the prescaler to 1024 by writing 0x5 (101) to the clock select bits
    tccr1bBits->clockSelect = 0x5;

    // Write the modified value back to the register
    *(volatile uint8_t*)TCCR1B_REG = currentValue;
}

static void set_ocr1a_compare_value()
{
    const uint16_t compareMatchVal = F_CPU / (PRESCALER * DESIRED_FREQUENCY) - 1;

    // Disable interrupts to ensure atomic access if enabled as we are performing
    // two writes to 16-bit register
    uint8_t sreg = get_status_reg();
    disable_interrupts();

    *(volatile uint16_t*)ORC1A_REG = compareMatchVal;

    // Restore status register
    set_status_reg(sreg);
}

static void enable_compare_a_match_interrupt()
{
    // Read the current value of the register
    uint8_t currentValue = *(volatile uint8_t*)TIMSK1_REG;

    // Create a bit field structure and assign the current value to it
    TIMSK1Bits* timsk1Bits = (TIMSK1Bits*)&currentValue;

    // Enable compare interrupt of output A compare
    timsk1Bits->compareAMatchEnable = 0x1;

    // Write the modified value back to the register
    *(volatile uint8_t*)TIMSK1_REG = currentValue;
}