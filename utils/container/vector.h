#ifndef UTILS_CONTAINER_VECTOR_H
#define UTILS_CONTAINER_VECTOR_H

#include "vector_defs.h"
 
// Initializes vector with desired element size (in bytes)
void vector_init(Vector* vector, uint64_t elem_size);

// Define a maximum element size
#define MAX_VEC_ELEMENT_SIZE 128

// Macros to simplify function calls
#define VECTOR_PUSH_BACK(vec, value) (vec).push_back(&(vec), &(value))
#define VECTOR_POP_FRONT(vec, type) ((type*)(vec).pop_front(&(vec)))
#define VECTOR_GET(vec, type, index) (*(type*)(vec).get(&(vec), index))
#define VECTOR_SIZE(vec) (vec).size(&(vec))
#define VECTOR_CLEAR(vec) (vec).clear(&(vec))

#endif // UTILS_CONTAINER_VECTOR_H