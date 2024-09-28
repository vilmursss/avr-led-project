#include "reg_interrupt.h"
#include "reg_interrupt_defs.h"

#include <register/reg_write.h>

#include <stdio.h>

// Registers
volatile uint8_t* const SREG_REG = (volatile uint8_t*)0x5F; // AVR Status Register

// Local functions
static void set_interrupt_state(uint8_t state);

void enable_interrupts()
{
    set_interrupt_state(1);
}

void disable_interrupts()
{
    set_interrupt_state(0);
}

uint8_t get_status_reg()
{
    return *(volatile uint8_t*)SREG_REG;
}

void set_status_reg(const uint8_t value)
{
    *(volatile uint8_t*)SREG_REG = value;
}

static void set_interrupt_state(const uint8_t state)
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