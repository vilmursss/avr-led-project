#include "reg_interrupt.h"
#include "reg_interrupt_defs.h"

// Registers
volatile uint8_t* const SREG_REG = (volatile uint8_t*)0x5F; // AVR Status Register

// Local functions
static void set_interrupt_state(uint8_t state);

void enable_interrupts()
{
    set_interrupt_state(0x1);
}

void disable_interrupts()
{
    set_interrupt_state(0x0);
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
        // Read the current value of the register
    uint8_t currentValue = *(volatile uint8_t*)SREG_REG;

    // Create a bit field structure and assign the current value to it
    SREGBits* sregBits = (SREGBits*)&currentValue;

    // Enable global interrupts
    sregBits->globalInterruptEnable = state;

    // Write the modified value back to the register
    *(volatile uint8_t*)SREG_REG = currentValue;
}