#ifndef UTILS_SYSTEM_CLOCK_H
#define UTILS_SYSTEM_CLOCK_H

#include <stdint.h>

// Perform required initialization options so timestamp can be retrieved
// If this is not called then timer_get_current_ts() returns only 00:00:00
void system_clock_init();

// Retrieve uptime since the system start in seconds
uint64_t system_clock_get_uptime();

#endif // UTILS_SYSTEM_CLOCK_H