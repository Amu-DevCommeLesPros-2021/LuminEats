#include "algorithm.h"

#include "vector/vector_types.h"
#include "vector/vector_api.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

bool all_of(
    iterator first,
    iterator last,
    bool (*predicate)(void const* data))
{
    for(; compare(first, last) != 0; increment(&first, 1))
    {
        if(!predicate(first.element))
        {
            return false;
        }
    }
    return true;
}

bool any_of(
    iterator first,
    iterator last,
    bool (*predicate)(void const* data))
{
    for(; compare(first, last) != 0; increment(&first, 1))
    {
        if(predicate(first.element))
        {
            return true;
        }
    }
    return false;
}

bool none_of(
    iterator first,
    iterator last,
    bool (*predicate)(void const* data))
{
    return !any_of(first, last, predicate);
}

void for_each(
    iterator first,
    iterator last,
    void (*unary_operation)(void* data))
{
    for(;compare(first, last) != 0; increment(&first, 1))
    {
        unary_operation(first.element);
    }
}

size_t count_if(
    iterator first,
    iterator last,
    bool (*unary_operation)(void const* data))
{
    size_t count = 0;

    for(;compare(first, last) != 0; increment(&first, 1))
    {
        if(unary_operation(first.element))
        {
            ++count;
        }
    }

    return count;
}

iterator find_if(
    iterator first,
    iterator last,
    bool (*predicate)(void const* data))
{
    for(;compare(first, last) != 0; increment(&first, 1))
    {
        if(predicate(first.element))
        {
            return first;
        }
    }

    return last;
}

iterator copy(
    iterator first,
    iterator last,
    iterator destination)
{
    for(;compare(first, last) != 0; increment(&first, 1))
    {
        memcpy(destination.element, first.element, first.element_size);
        increment(&destination, 1);
    }

    return destination;
}

iterator copy_if(
    iterator first,
    iterator last,
    iterator destination,
    bool (*predicate)(void const* data))
{
    for(;compare(first, last) != 0; increment(&first, 1))
    {
        if(predicate(first.element))
        {
            memcpy(destination.element, first.element, first.element_size);
            increment(&destination, 1);
        }
    }

    return destination;
}

void fill(
    iterator first,
    iterator last,
    void const* data)
{
    for(;compare(first, last) != 0; increment(&first, 1))
    {
        memcpy(first.element, data, first.element_size);
    }
}

iterator transform(
    iterator first,
    iterator last,
    iterator destination,
    void (*unary_operation)(void* data))
{
    for(;compare(first, last) != 0; increment(&first, 1))
    {
        memcpy(destination.element, first.element, first.element_size);
        unary_operation(destination.element);
        increment(&destination, 1);
    }

    return destination;
}

void generate(
    iterator first,
    iterator last, void* (*generator)())
{
    for(;compare(first, last) != 0; increment(&first, 1))
    {
        memcpy(first.element, generator(), first.element_size);
    }
}

iterator rotate(
    iterator first,
    iterator n_first,
    iterator last)
{
    void* rotated = malloc(last.element - first.element);

    memcpy(rotated, n_first.element, last.element - n_first.element);
    memcpy(rotated + (last.element - n_first.element), first.element, n_first.element - first.element);
    memcpy(first.element, rotated, last.element - first.element);
    
    free(rotated);

    return (iterator){
        .element = first.element + (last.element - n_first.element),
        .element_size = first.element_size
    };
}

void reverse(
    iterator first,
    iterator last)
{
    for(;compare(first, last) != 0; increment(&first, 1))
    {
        iterator back = last;
        decrement(&back, 1);
        rotate(first, back, last);
    }
}

int compare_int(
    const void* a,
    const void* b)
{
    return *(int*)a - *(int*)b;
}

void sort(
    iterator first,
    iterator last)
{
    qsort(first.element, (last.element - first.element) / first.element_size, first.element_size, compare_int);
}

void sort_by(
    iterator first,
    iterator last,
    int (*comparator)(void const* a, void const* b))
{
    qsort(first.element, (last.element - first.element) / first.element_size, first.element_size, comparator);
}

iterator unique(
    iterator first,
    iterator last,
    int (*comparator)(void const* a, void const* b))
{
    if(compare(first, last) == 0)
    {
        return last;
    }

    sort_by(first, last, compare_int);

    iterator result = first;
    for(increment(&first, 1); compare(first, last) != 0; increment(&first, 1))
    {
        if(comparator(result.element, first.element))
        {
            increment(&result, 1);
            if(compare(result, first) != 0)
            {
                memcpy(result.element, first.element, first.element_size);
            }
        }
    }

    increment(&result, 1);
    return result;
}

bool a_precedes_b(
    void const* a,
    void const* b,
    int (*comparator)(void const* a, void const* b))
{
    return comparator(a, b) < 0;
}

bool b_does_not_precede_a(
    void const* a,
    void const* b,
    int (*comparator)(void const* a, void const* b))
{
    return comparator(b, a) >= 0;
}

iterator bound(
    iterator first,
    iterator last,
    void const* value,
    int (*comparator)(void const* a, void const* b),
    bool (*predicate)(void const* a, void const* b, int (*)(void const* a, void const* b)))
{
    iterator i;
    size_t count = (last.element - first.element) / first.element_size, step;

    while(count > 0)
    {
        i = first;
        step = count / 2;
        increment(&i, step);
        if(predicate(i.element, value, comparator))
        {
            increment(&i, 1);
            first = i;
            count -= step + 1;
        }
        else
        {
            count = step;
        }
    }

    return first;
}

iterator lower_bound(
    iterator first,
    iterator last,
    void const* value, int (*comparator)(void const* a, void const* b))
{
    return bound(first, last, value, comparator, a_precedes_b);
}

iterator upper_bound(
    iterator first,
    iterator last,
    void const* value, int (*comparator)(void const* a, void const* b))
{
    return bound(first, last, value, comparator, b_does_not_precede_a);
}

bool binary_search(
    iterator first,
    iterator last,
    void const* value, int (*comparator)(void const* a, void const* b))
{
    first = lower_bound(first, last, value, comparator);

    return (compare(first, last) != 0 && !comparator(value, first.element));
}

iterator_interval equal_range(
    iterator first,
    iterator last,
    void const* value, int (*comparator)(void const* a, void const* b))
{
    return (iterator_interval){
        .begin = lower_bound(first, last, value, comparator),
        .end = upper_bound(first, last, value, comparator)
    };
}

iterator set_difference(
    iterator first1,
    iterator last1,
    iterator first2,
    iterator last2,
    iterator destination,
    int (*comparator)(void const* a, void const* b))
{
    while(compare(first1, last1) != 0)
    {
        if(compare(first2, last2) == 0)
        {
            memcpy(destination.element, first1.element, last1.element - first1.element);
            increment(&destination, (last1.element - first1.element) / first1.element_size);
            return destination;
        }

        if(comparator(first1.element, first2.element) < 0)
        {
            set(destination, first1.element);
            increment(&first1, 1);
            increment(&destination, 1);
        }
        else
        {
             if(comparator(first2.element, first1.element) >= 0)
             {
                 increment(&first1, 1);
             }
             increment(&first2, 1);
        }
    }

    return destination;
}

iterator set_intersection(
    iterator first1,
    iterator last1,
    iterator first2,
    iterator last2,
    iterator destination,
    int (*comparator)(void const* a, void const* b))
{
    while(compare(first1, last1) != 0 && compare(first2, last2) != 0)
    {
        if(comparator(first1.element, first2.element) < 0)
        {
            increment(&first1, 1);
        }
        else
        {
            if(comparator(first2.element, first1.element) >= 0)
            {
                set(destination, first1.element);
                increment(&destination, 1);
                increment(&first1, 1);
            }
            increment(&first2, 1);
        }
    }

    return destination;
}
