#include "reg_status.h"
#include "reg_status_defs.h"

#include <writer/reg_writer.h>

#include <stdio.h>

// Registers
volatile uint8_t* const SREG_REG = (volatile uint8_t*)0x5F; // AVR Status Register

// Local functions
static void set_global_interrupts(uint8_t state);

void reg_status_enable_interrupts()
{
    set_global_interrupts(1);
}

void reg_status_disable_interrupts()
{
    set_global_interrupts(0);
}

uint8_t reg_status_get()
{
    return *(volatile uint8_t*)SREG_REG;
}

void reg_status_set(const uint8_t value)
{
    *(volatile uint8_t*)SREG_REG = value;
}

static void set_global_interrupts(const uint8_t state)
{
    // Change global interrupts state
    SREGBits sreg = {0};
    sreg.globalInterruptEnable = state;

    const WriteStatus ret = reg_write_bits(SREG_REG, &sreg, REG_SIZE_8);
    if (ret!= WRITE_OK)
    {
        printf("ERROR(%d): Failed to write SREGBits to SREG_REG", ret);
    }
}