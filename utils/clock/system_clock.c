#include "system_clock.h"
#include "common_clock_defs.h"

#include <log/logger.h>
#include <status/reg_status.h>
#include <writer/reg_writer.h>

// Define the clock frequency if not already defined
#ifndef F_CPU
    #define F_CPU 16000000UL // 16 MHz
#endif

// Constants
static const uint8_t DESIRED_FREQUENCY = 1; // 1 Hz
static const uint32_t SYS_CLOCK_PRESCALER = 1024;

// Registers
volatile uint8_t* const TIMSK1_REG = (volatile uint8_t*)0x6F; // Timer/Counter1 Interrupt Flag Register
volatile uint8_t* const TCCR1B_REG = (volatile uint8_t*)0x81; // Timer/Counter1 Control Register B
volatile uint16_t* const OCR1B_REG = (volatile uint16_t*)0x88; //  Timer/Counter1 - Combinced output compare register B for low & high byte

// Local variables
static volatile uint64_t system_uptime = 0;

// Local functions
static void set_ctc_mode_on();
static void set_clock_select_bits();
static void set_ocr1a_compare_value();
static void enable_compare_a_match_interrupt();

// ISR for Timer/Counter1 Compare Match A (Vector 11)
void __vector_11(void) __attribute__ ((signal, used, externally_visible));
void __vector_11(void)
{
    system_uptime++;
}

void system_clock_init()
{
    set_ctc_mode_on();
    set_clock_select_bits();
    set_ocr1a_compare_value();
    enable_compare_a_match_interrupt();
    reg_status_enable_interrupts();
}

uint64_t system_clock_get_uptime()
{
    return system_uptime;
}

static void set_ctc_mode_on()
{
    // Set CTC mode
    TCCR1Bits tccr1bBits = {0};
    tccr1bBits.waveformGenMode = 0x1;

    const WriteStatus ret = reg_write_bits(TCCR1B_REG, &tccr1bBits, REG_SIZE_8);
        if (ret != WRITE_OK)
    {
        log_warning("Failed to write TCCR1Bits (Set CTC mode) to TCCR1B_REG (Error Code: %d)", ret);
    }
}

static void set_clock_select_bits()
{
    // Select desired clock for this PRESCALER
    TCCR1Bits tccr1bBits = {0};
    tccr1bBits.clockSelect = 0x5;

    const WriteStatus ret = reg_write_bits(TCCR1B_REG, &tccr1bBits, REG_SIZE_8);
        if (ret != WRITE_OK)
    {
        log_warning("Failed to write TCCR1Bits (Set clock select) to TCCR1B_REG (Error Code: %d)", ret);
    }
}

static void set_ocr1a_compare_value()
{
    // Calculate compare match value
    uint16_t compareMatchVal = F_CPU / (SYS_CLOCK_PRESCALER * DESIRED_FREQUENCY) - 1;

    // Disable interrupts to ensure atomic access if enabled as we are performing
    // two writes to 16-bit register
    uint8_t sreg = reg_status_get();
    reg_status_disable_interrupts();

    const WriteStatus ret = reg_write_bits(OCR1B_REG, &compareMatchVal, REG_SIZE_16);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write CompareMatchVAl to ORC1A_REG (Error Code: %d)", ret);
    }

    // Restore status register
    reg_status_set(sreg);
}

static void enable_compare_a_match_interrupt()
{
    TIMSK1Bits timsk1 = {0};
    timsk1.compareAMatchEnable = 0x1;

    const WriteStatus ret = reg_write_bits(TIMSK1_REG, &timsk1, REG_SIZE_8);
    if (ret != WRITE_OK)
    {
        log_warning("Failed to write TIMSK1Bits to TIMSK1_REG (Error Code: %d)", ret);
    }
}