#include "vector.h"

#include <stdlib.h>
#include <string.h>

// Vector functions
static VecStatus vector_push_back(Vector* self, void* value);
static void* vector_pop_front(Vector* self);
static void* vector_get(Vector* self, uint64_t index);
static void vector_clear(Vector* self);
static uint64_t vector_size(Vector* self);

// Function to initialize the vector
void vector_init(Vector* vector, const uint64_t elem_size)
{
    vector->data = NULL;
    vector->num_elements = 0;
    vector->capacity = 0;
    vector->elem_size = elem_size;

    // Assign function pointers
    vector->push_back = vector_push_back;
    vector->pop_front = vector_pop_front;
    vector->get = vector_get;
    vector->size = vector_size;
    vector->clear = vector_clear;
}

// Vector functions
static VecStatus vector_push_back(Vector* self, void* value)
{
    if (self->num_elements >= MAX_VEC_ELEMENT_SIZE)
    {
        return VEC_FULL;
    }

    if (self->num_elements == self->capacity)
    {
        self->capacity = self->capacity == 0 ? 1 : self->capacity * 2;
        self->data = realloc(self->data, self->capacity * self->elem_size);
        if (self->data == NULL)
        {
            return VEC_DATA_ALLOCATION_FAILURE;
        }
    }
    memcpy((char*)self->data + self->num_elements * self->elem_size, value, self->elem_size);
    self->num_elements++;

    return VEC_OK;
}

static void* vector_pop_front(Vector* self)
{
    if (self->num_elements == 0)
    {
        return NULL;
    }

    void *value = self->data;

    memmove(self->data, (char*)self->data + self->elem_size, (self->num_elements - 1) * self->elem_size);
    self->num_elements--;

    return value;
}

static void* vector_get(Vector* self, const uint64_t index)
{
    if (index >= self->num_elements)
    {
        return NULL;
    }
    return (char*)self->data + index * self->elem_size;
}

static uint64_t vector_size(Vector* self)
{
    return self->num_elements;
}

static void vector_clear(Vector* self)
{
    free(self->data);
    self->data = NULL;
    self->num_elements = 0;
    self->capacity = 0;
}