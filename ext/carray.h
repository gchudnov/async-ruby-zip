#ifndef EXTRA_CARRAY_H
#define EXTRA_CARRAY_H

#include <stddef.h>
#include <stdio.h>

/* CARRAY for strings*/
typedef struct
{
    char** ptr;
    size_t size;
} carray_str_t;

/* Creation & Destruction */
carray_str_t* carray_str_create(size_t count);
void carray_str_destroy(carray_str_t* arr);

/* Utilities */
size_t carray_str_size(const carray_str_t* arr);

/* Accessors */
const char* carray_str_get(const carray_str_t* arr, size_t index);
char* carray_str_set(carray_str_t* arr, size_t index, const char* data);

void carray_str_unset(carray_str_t* arr, size_t index);


#endif
