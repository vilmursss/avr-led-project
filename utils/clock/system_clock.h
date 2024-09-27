#ifndef UTILS_SYSTEM_CLOCK_H
#define UTILS_SYSTEM_CLOCK_H

// Perform required initialization options so timestamp can be retrieved
// If this is not called then timer_get_current_ts() returns only 00:00:00
void system_clock_init();

// Retrieve timestamp for the log print. Buffer size is max 10 bytes.
// Uses format "HH:MM:SS" to represent time since the microcontroller was started
const char* system_clock_get_ts();

#endif // UTILS_SYSTEM_CLOCK_H