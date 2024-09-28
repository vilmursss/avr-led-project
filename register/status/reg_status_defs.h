#ifndef REGISTER_REG_STATUS_DEFS_H
#define REGISTER_REG_STATUS_DEFS_H

#include <stdint.h>

typedef struct {
    uint8_t carryFlag : 1;             // Bit 0: Carry flag
    uint8_t zeroFlag : 1;              // Bit 1: Zero flag
    uint8_t negativeFlag : 1;          // Bit 2: Negative flag
    uint8_t twoComplementFlag : 1;     // Bit 3: Two's complement overflow flag  
    uint8_t signBit : 1;               // Bit 4: Sign bit
    uint8_t halfCarryFlag : 1;         // Bit 5: Half Carry Flag
    uint8_t bitCopyStorage : 1;        // Bit 6: Bit copy storage instructions load flag
    uint8_t globalInterruptEnable : 1; // Bit 7: Enable global interrupts
} SREGBits;

#endif // REGISTER_REG_STATUS_DEFS_H