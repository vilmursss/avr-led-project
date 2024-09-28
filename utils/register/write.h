#ifndef UTILS_REGISTER_WRITE_H
#define UTILS_REGISTER_WRITE_H

#include "reg_write_defs.h"

// Function used to write desired bits to register using OR |= operation
WriteStatus reg_write_bits(volatile void* registerAddress, void* newBits, RegisterSize size);

#endif // UTILS_REGISTER_WRITE_H