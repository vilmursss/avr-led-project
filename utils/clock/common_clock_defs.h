#ifndef SYSTEM_CLOCK_DEFS_H
#define SYSTEM_CLOCK_DEFS_H

#include <stdint.h>

// Common registers
extern volatile uint8_t* const TIMSK1_REG;

typedef struct {
    uint8_t clockSelect : 3;               // Bits 0-2: Clock Select
    uint8_t waveformGenMode : 2;           // Bits 3-4: Waveform Generation Mode
    uint8_t reserved : 1;                  // Bit 5: Not used (Reserved for the future)
    uint8_t inputCaptureEdgeSelect : 1;    // Bit 6: Input Capture Edge Select
    uint8_t inputCaptureNoiseCanceler : 1; // Bit 7: Input Capture Noise Canceler
} TCCR1Bits;

typedef struct {
    uint8_t overflowEnable : 1;      // Bit 0: Overflow Interrupt Enable
    uint8_t compareAMatchEnable : 1; // Bit 1: Output Compare A Match Interrupt Enable
    uint8_t compareBMatchEnable : 1; // Bit 2: Output Compare B Match Interrupt Enable
    uint8_t reserved_1 : 2;          // Bits 3-4: Reserved (Typically always read as zero)
    uint8_t inputCaptureEnable : 1;  // Bit 5: Input Capture Interrupt Enable
    uint8_t reserved_2 : 2;          // Bits 6-7: Reserved (Typically always read as zero)
} TIMSK1Bits;

#endif // SYSTEM_CLOCK_DEFS_H