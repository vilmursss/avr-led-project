#ifndef UTILS_SYSTEM_CLOCK_H
#define UTILS_SYSTEM_CLOCK_H

#include <stdint.h>

// Initializes vector 11 timer interrupts so the uptime counter starts ticking.
void system_clock_init();

// Retrieve uptime since the system start in seconds
uint64_t system_clock_get_uptime();

#endif // UTILS_SYSTEM_CLOCK_H