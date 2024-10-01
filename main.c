#include <clock/system_clock.h>
#include <log/logger.h>
#include <serial/stdout_redirect.h>

#include <util/delay.h>

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

    while (1)
    {
        log_debug("Test print\n");
        _delay_ms(1000);
    }

    return 0;
}