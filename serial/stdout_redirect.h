#ifndef SERIAL_STDOUT_REDIRECT_H
#define SERIAL_STDOUT_REDIRECT_H

#include <stdint.h>

// Initializes serial bus so that the stdout is redirected to serial
// bus with given baud rate.
// Note that buffer size is max 100 bytes and it will be send always
// to serial if it becomes full. Currently line change is deciding
// factor which causes the stdout to be redirected or previously mentioned
// maximum buffer size
void stdout_redirect_init(uint32_t baud);

#endif // SERIAL_STDOUT_REDIRECT_H