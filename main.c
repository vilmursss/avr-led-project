#include <util/delay.h>
#include <stdio.h>

// Check if serial debug baud rate is given through the CFLAGS
#ifndef SERIAL_BAUD
    #define SERIAL_BAUD 9600
#endif

int main(void) {
    while (1)
    {
        // Send debug message using printf
        printf("Test print\n");
        _delay_ms(1000);
    }

    return 0;
}