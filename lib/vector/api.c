#include "vector/api.h"

#include "vector/types.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void grow(
    vector* v,
    size_t const needed)
{
    if(v->capacity == 0)
    {
        v->capacity = needed;
        v->data = malloc(v->capacity * v->element_size);
    }
    else
    {
        while(v->capacity < needed)
        {
            v->capacity *= v->growth_factor;
            v->data = realloc(v->data, v->capacity * v->element_size);
        }
    }
}

vector make_vector(
    size_t const element_size,
    size_t const num_elements)
{
    return (vector){
            .element_size = element_size,
            .growth_factor = 2.0,
            .data = num_elements ? malloc(num_elements * element_size) : NULL,
            .size = num_elements,
            .capacity = num_elements,
            };
}

void destroy(
    vector* v)
{
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

size_t size(
    vector const v)
{
    return v.size;
}

size_t capacity(
    vector const v)
{
    return v.capacity;
}

void push_back(
    vector* v,
    void const* data)
{
    insert(v, end(v), data);
}

void* pop_back(
    vector* v)
{
    iterator i = end(v);
    decrement(&i, 1);
    erase(v, i);
    return v->data + v->size * v->element_size;
}

void* back(
    vector* v)
{
    return v->data + (v->size - 1) * v->element_size;
}

void insert(
    vector* v,
    iterator const i,
    void const* data)
{
    size_t const chunk_offset = i.element - v->data;     // En octets.
    size_t const chunk_size = end(v).element - i.element;    // En octets.
    
    grow(v, v->size + 1);

    memmove(v->data + chunk_offset + v->element_size, v->data + chunk_offset, chunk_size);
    memcpy(v->data + chunk_offset, data, v->element_size);
    ++v->size;
}

void erase(
    vector* v,
    iterator const i)
{
    size_t const chunk_offset = i.element - v->data;     // En octets.
    size_t const chunk_size = end(v).element - i.element - v->element_size;    // En octets.

    memmove(v->data + chunk_offset, v->data + chunk_offset + v->element_size, chunk_size);
    --v->size;
}

void keep_if(
    vector* v,
    bool (*binary_predicate)(void const* a, void const* b),
    void const* b)
{
    for(void *d = v->data; d != v->data + v->size * v->element_size;)
    {
        if(!binary_predicate(d, b))
        {
            size_t const chunk_offset = d - v->data;     // En octets.
            size_t const chunk_size = end(v).element - d - v->element_size;    // En octets.

            memmove(v->data + chunk_offset, v->data + chunk_offset + v->element_size, chunk_size);
            --v->size;
        }
        else
        {
            d += v->element_size;
        }
    }

}

void assign(
    vector* v,
    iterator first,
    iterator last)
{
    v->size = (last.element - first.element) / v->element_size;
    grow(v, v->size);

    memcpy(v->data, first.element, last.element - first.element);
}

void clear(
    vector* v)
{
    v->size = 0;
}

void swap(
    vector* a,
    vector* b)
{
    if(a != b)
    {
        void *t = b->data;
        b->data = a->data;
        a->data = t;

        a->size ^= b->size;
        b->size ^= a->size;
        a->size ^= b->size;

        a->capacity ^= b->capacity;
        b->capacity ^= a->capacity;
        a->capacity ^= b->capacity;
    }
}

void reserve(
    vector* v,
    size_t const num_elements)
{
    grow(v, num_elements);
}

void resize(
    vector* v,
    size_t const num_elements)
{
    if(v->size < num_elements)
    {
        v->size = v->capacity = num_elements;
        v->data = realloc(v->data, v->capacity * v->element_size);
    }
    else if(v->size > num_elements)
    {
        v->size = num_elements;
    }
}

void shrink_to_fit(
    vector* v)
{
    if(v->capacity > v->size)
    {
        v->capacity = v->size;
        v->data = realloc(v->data, v->capacity * v->element_size);
    }
}

iterator begin(
    vector const* v)
{
    return (iterator){
            .element = v->data,
            .element_size = v->element_size,
            };
}

iterator end(
    vector const* v)
{
    return (iterator){
            .element = v->data + v->element_size * v->size,
            .element_size = v->element_size,
            };
}

iterator at(
    vector const* v,
    size_t const index)
{
    assert(index < v->size);

    return (iterator){
            .element = v->data + v->element_size * index,
            .element_size = v->element_size,
            };
}

int compare(
    iterator const a,
    iterator const b)
{
    return a.element - b.element;
}

void increment(
    iterator* i,
    size_t const num_elements)
{
    i->element += i->element_size * num_elements;
}

void decrement(
    iterator* i,
    size_t const num_elements)
{
    i->element -= i->element_size * num_elements;
}

void* value(
    iterator const i)
{
    return i.element;
}

void set(
    iterator i,
    void const* data)
{
    memcpy(i.element, data, i.element_size);
}
