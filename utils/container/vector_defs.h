#ifndef UTILS_CONTAINER_VECTOR_DEFS_H
#define UTILS_CONTAINER_VECTOR_DEFS_H

#include <stdint.h>

struct Vector;

// Enum to represent function return codes
typedef enum {
    VEC_OK = 0,
    VEC_FULL = -1,
    VEC_DATA_ALLOCATION_FAILURE = -2,
} VecStatus;

typedef struct Vector
{
    void *data;            // Pointer to the array of elements
    uint64_t num_elements; // Current number of elements
    uint64_t capacity;     // Current allocated capacity
    uint64_t elem_size;    // Size of each element

    // Function pointers for operations
    VecStatus (*push_back)(struct Vector* self, void* value);
    void* (*pop_front)(struct Vector* self);
    void* (*get)(struct Vector* self, uint64_t index);
    void (*clear)(struct Vector* self);
    uint64_t (*size)(struct Vector* self);
} Vector;

#endif // UTILS_CONTAINER_VECTOR_DEFS_H