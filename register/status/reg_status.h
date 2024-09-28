#ifndef REGISTER_REG_STATUS_H
#define REGISTER_REG_STATUS_H

#include <stdint.h>

// Enables global interrupts
void reg_status_enable_interrupts();

// Disables global interrupts
void reg_status_disable_interrupts();

// Get current status register
uint8_t reg_status_get();

// Set value to status register
void reg_status_set(uint8_t value);

#endif // REGISTER_REG_STATUS_H