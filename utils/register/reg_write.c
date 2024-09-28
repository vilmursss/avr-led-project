#include "reg_write.h"

#include <stddef.h>

WriteStatus reg_write_bits(
    volatile void* registerAddress, void* newBits, const RegisterSize size)
{
    if (registerAddress == NULL)
    {
        return WRITE_INVALID_REG_ADDRESS;
    }

    if (newBits == NULL)
    {
        return WRITE_INVALID_BIT_ADDRESS;
    }

    WriteStatus ret = WRITE_OK;

    // Read the current value of the register
    switch (size)
    {
        case REG_SIZE_8:
            *(volatile uint8_t*)registerAddress |= *(uint8_t*)newBits;
            break;
        case REG_SIZE_16:
            *(volatile uint16_t*)registerAddress |= *(uint16_t*)newBits;
            break;
        case REG_SIZE_32:
            *(volatile uint32_t*)registerAddress |= *(uint32_t*)newBits;
            break;
        case REG_SIZE_64:
            *(volatile uint64_t*)registerAddress |= *(uint64_t*)newBits;
            break;
        default:
            ret = WRITE_UNKNOWN_REG_SIZE;
    }

    return ret;
}