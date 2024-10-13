#ifndef PWM_BRIGHTNESS_CONTROL_DEFS_H
#define PWM_BRIGHTNESS_CONTROL_DEFS_H

#include <stdint.h>

// Both 8-bit registers has same controlling bits
typedef struct {
    uint8_t clockSelect : 3;      // Bits 0-2: Clock Select
    uint8_t waveformGenMode : 1;  // Bit 3: Waveform Generation Mode
    uint8_t reserved : 2;         // Bits 4-5: Not used (Reserved for the future)
    uint8_t forceOutputCompB : 1; // Bit 6: Force Output Compare B
    uint8_t forceOutputCompA : 1; // Bit 7: Force Output Compare A
} TCCR2BBits;

typedef struct {
    uint8_t waveformGenMode : 2;    // Bits 0-1: Waveform Generation Mode
    uint8_t reserved : 2;           // Bits 2-3: Reserved bits
    uint8_t compareOutputModeB : 2; // Bits 4-5: Compare Match Output B Mode
    uint8_t compareOutputModeA : 2; // Bits 6-7: Compare Match Output A Mode
} TCCR2ABits;

// Enum for status codes
typedef enum {
    PWM_SUCCESS,
    PWM_FAILURE_INVALID_COLOR,
    PWM_FAILURE_HIGHER_CURRENT_BRIGHTNESS,
    PWM_FAILURE_TOO_LARGE_DURATION,
    PWM_FAILURE_GPIO_NOT_OUTPUT,
} PWMStatus;

typedef enum {
    PWM_RED,
    PWM_GREEN,
    PWM_BLUE,
    PWM_COLOR_UNKNOWN
} PWMColor;

#endif // PWM_BRIGHTNESS_CONTROL_DEFS_H