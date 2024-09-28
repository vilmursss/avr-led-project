#ifndef UTILS_REGISTER_WRITE_DEFS_H
#define UTILS_REGISTER_WRITE_DEFS_H

#include <stdint.h>

// Enum to specify register size
typedef enum {
    REG_SIZE_8,
    REG_SIZE_16,
    REG_SIZE_32,
    REG_SIZE_64,
} RegisterSize;

// Enum to represent write status
typedef enum {
    WRITE_OK = 0,
    WRITE_INVALID_REG_ADDRESS = -1,
    WRITE_INVALID_BIT_ADDRESS = -2,
    WRITE_UNKNOWN_REG_SIZE = -3,
} WriteStatus;

#endif // UTILS_REGISTER_WRITE_DEFS_H