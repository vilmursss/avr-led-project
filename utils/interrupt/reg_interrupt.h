#ifndef UTILS_REG_INTERRUPT_H
#define UTILS_REG_INTERRUPT_H

#include <stdint.h>

// Enables global interrupts
void enable_interrupts();

// Disables global interrupts
void disable_interrupts();

// Get current status register
uint8_t get_status_reg();

// Set value to status register
void set_status_reg(uint8_t value);

#endif // UTILS_INTERRUPT_H