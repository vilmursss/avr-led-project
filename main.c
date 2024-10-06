#include <clock/system_clock.h>
#include <clock/timer.h>
#include <led/rgb.h>
#include <log/logger.h>
#include <serial/stdout_redirect.h>

// Check if serial debug baud rate is given through the CFLAGS
#ifndef SERIAL_BAUD
    #define SERIAL_BAUD 9600
#endif

int main(void)
{
    // Init and start uptime counter
    system_clock_init();

    // Init stdout redirection to serial
    stdout_redirect_init(SERIAL_BAUD);

    rgb_setup();
    rgb_test_all_colors();

    return 0;
}