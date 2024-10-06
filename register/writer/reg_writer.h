#ifndef REGISTER_REG_WRITER_H
#define REGISTER_REG_WRITER_H

#include "reg_writer_defs.h"

// Function used to write desired bits to register using OR |= operation
WriteStatus reg_write_bits(volatile void* registerAddress, void* newBits, RegisterSize size);

// Function used to clear register and then write desired bits
WriteStatus reg_clear_and_write_bits(
    volatile void* registerAddress, void* newBits, const RegisterSize size);

#endif // REGISTER_REG_WRITER_H