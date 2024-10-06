#ifndef LED_RGB_DEFS_H
#define LED_RGB_DEFS_H

#include <stdint.h>

typedef struct {
    uint8_t PB0 : 1; // Bit 0: PORTB pin 0
    uint8_t PB1 : 1; // Bit 1: PORTB pin 1
    uint8_t PB2 : 1; // Bit 2: PORTB pin 2
    uint8_t PB3 : 1; // Bit 3: PORTB pin 3
    uint8_t PB4 : 1; // Bit 4: PORTB pin 4
    uint8_t PB5 : 1; // Bit 5: PORTB pin 5
    uint8_t PB6 : 1; // Bit 6: PORTB pin 6
    uint8_t PB7 : 1; // Bit 7: PORTB pin 7
} PORTBBits;

typedef struct {
    uint8_t DB0 : 1; // Bit 0: Data Direction of PORTB pin 0
    uint8_t DB1 : 1; // Bit 1: Data Direction of PORTB pin 1
    uint8_t DB2 : 1; // Bit 2: Data Direction of PORTB pin 2
    uint8_t DB3 : 1; // Bit 3: Data Direction of PORTB pin 3
    uint8_t DB4 : 1; // Bit 4: Data Direction of PORTB pin 4
    uint8_t DB5 : 1; // Bit 5: Data Direction of PORTB pin 5
    uint8_t DB6 : 1; // Bit 6: Data Direction of PORTB pin 6
    uint8_t DB7 : 1; // Bit 7: Data Direction of PORTB pin 7
} DDRBBits;

#endif // LED_RGB_DEFS_H