#include "timer.h"
#include "common_clock_defs.h"

#include <log/logger.h>
#include <status/reg_status.h>
#include <writer/reg_writer.h>

#include <stdbool.h>

// Define the clock frequency if not already defined
#ifndef F_CPU
    #define F_CPU 16000000UL // 16 MHz
#endif

// Registers
volatile uint8_t* const TCCR0A_REG = (volatile uint8_t*)0x44; // Timer/Counter0 - Control Register A
volatile uint8_t* const TCCR0B_REG = (volatile uint8_t*)0x45; // Timer/Counter1 - Control Register B
volatile uint8_t* const TCNT0_REG = (volatile uint8_t*)0x46;  // Timer/Counter0 - Counter Register
volatile uint8_t* const OCR0A_REG = (volatile uint8_t*)0x47;  // Timer/Counter0 - Output Compare Register A
volatile uint8_t* const TIMSK0_REG = (volatile uint8_t*)0x6E; // Timer/Counter0 - Interrupt Flag Register

// Local functions
static void configure_control_registers();
static void set_1_ms_compare_value();
static void set_compare_a_match_interrupt(bool enable);

// Local variables
volatile uint16_t interruptTicks = 0;
volatile uint16_t targetTicks = 0;
static const uint32_t TIMER_PRESCALER = 1024;

// ISR for Timer/Counter0 compare match A (Vector 14)
void __vector_14(void) __attribute__ ((signal, used, externally_visible));
void __vector_14(void)
{
    interruptTicks++;
}

void timer_delay_ms(uint16_t delay_ms)
{
    // Reset counter register
    *TCNT0_REG = 0;

    // Setup registers and interrupts
    configure_control_registers();
    set_1_ms_compare_value();

    // Set target ticks for desired delay
    targetTicks = delay_ms;
    interruptTicks = 0;

    // Enable the output compare match A interrupt
    set_compare_a_match_interrupt(true);

    // Wait until interrupt ticks reaches the target
    while (interruptTicks < targetTicks);

    // Disable the output compare match A interrupt
    set_compare_a_match_interrupt(false);

    // Reset local variables
    interruptTicks = 0;
    targetTicks = 0;
}

static void configure_control_registers()
{
    // Select desired clock for this PRESCALER
    TCCR0BBits tccr0bBits = {0};
    tccr0bBits.clockSelect = 0x5;

    // Set CTC mode
    tccr0bBits.waveformGenMode = 0x1;

    WriteStatus ret = reg_write_bits(TCCR0B_REG, &tccr0bBits, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write TCCR0BBits (Set clock select) to TCCR0B_REG (Error Code: %d)\n", ret);
    }

    TCCR0ABits tccr0aBits = {0};

    // Set CTC mode
    tccr0aBits.waveformGenMode = 0x2;

    ret = reg_write_bits(TCCR0A_REG, &tccr0aBits, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write TCCR0ABits (Set clock select) to TCCR0A_REG (Error Code: %d)\n", ret);
    }
}

static void set_1_ms_compare_value()
{
    // Calculate the number of timer ticks needed for 1 ms
    uint8_t compare_value = (F_CPU / (TIMER_PRESCALER * 1000)) - 1;

    WriteStatus ret = reg_write_bits(OCR0A_REG, &compare_value, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write CompareMatchVal to OCR1BH_REG (Error Code: %d)\n", ret);
    }
}

static void set_compare_a_match_interrupt(const bool enable)
{
    TIMSK0Bits timsk0 = {0};
    timsk0.compareAMatchEnable = enable;

    const WriteStatus ret = reg_write_bits(TIMSK0_REG, &timsk0, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write TIMSK0Bits to TIMSK0_REG (Error Code: %d)\n", ret);
    }
}