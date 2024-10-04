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
volatile uint16_t* const OCR1B_REG = (volatile uint16_t*)0x8A; // Timer/Counter1 - Combined output compare register B for low & high byte

// Local functions
static void set_ocr1b_compare_value(uint16_t ticks);
static void set_compare_b_match_interrupt(bool enable);

// Local variables
static volatile uint8_t delay_flag = 0;
static const uint32_t PRESCALER = 1024;

// ISR for Timer/Counter1 Compare Match B (Vector 12)
void __vector_12(void) __attribute__ ((signal, used, externally_visible));
void __vector_12(void)
{
    delay_flag = 1;
}

void timer_delay_ms(uint16_t delay_ms)
{
    // Calculate the number of timer ticks needed for the delay
    const uint16_t ticks = F_CPU / (PRESCALER * (1000 / delay_ms)) - 1;
    set_ocr1b_compare_value(ticks);

    // Enable the output compare match B interrupt
    set_compare_b_match_interrupt(true);

    // Wait for the delay flag to be set
    while (!delay_flag);

    // Disable the output compare match B interrupt
    set_compare_b_match_interrupt(false);

    // Reset the delay flag
    delay_flag = 0;
}

static void set_ocr1b_compare_value(uint16_t ticks)
{
    // Disable interrupts to ensure atomic access if enabled as we are performing
    // two writes to 16-bit register
    uint8_t sreg = reg_status_get();
    reg_status_disable_interrupts();

    const WriteStatus ret = reg_write_bits(OCR1B_REG, &ticks, REG_SIZE_16);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write CompareMatchVal to OCR1B_REG (Error Code: %d)", ret);
    }

    // Restore status register
    reg_status_set(sreg);
}

static void set_compare_b_match_interrupt(const bool enable)
{
    // Enable compare match B
    TIMSK1Bits timsk1 = {0};
    timsk1.compareBMatchEnable = enable;

    const WriteStatus ret = reg_write_bits(TIMSK1_REG, &timsk1, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write TIMSK1Bits to TIMSK1_REG (Error Code: %d)", ret);
    }
}