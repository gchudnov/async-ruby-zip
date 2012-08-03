#include "carray.h"
#include <stdlib.h>
#include <string.h>

//
carray_str_t* carray_str_create(size_t count)
{
    carray_str_t* parr = (carray_str_t*)malloc(sizeof(carray_str_t));
    if(parr)
    {
        parr->ptr = (char**)malloc(sizeof(char*) * count);
        parr->size = count;

        memset(parr->ptr, 0, sizeof(char*) * count);
    }

    return parr;
}

//
void carray_str_destroy(carray_str_t* parr)
{
    if(parr)
    {
        size_t i;
        for(i = 0; i != parr->size; ++i)
            free(parr->ptr[i]);

        free(parr->ptr);
        free(parr);
    }
}

//
size_t carray_str_size(carray_str_t* arr)
{
    return arr->size;
}

//
const char* carray_str_get(const carray_str_t* arr, size_t index)
{
    if(index >= arr->size)
        return NULL;

    return arr->ptr[index];
}

//
char* carray_str_set(carray_str_t* arr, size_t index, const char* data)
{
    if(index >= arr->size)
        return NULL;

    if(arr->ptr)
        carray_str_unset(arr, index);

    if(data)
    {
        arr->ptr[index] = strdup(data);
    }

    return arr->ptr[index];
}

//
void carray_str_unset(carray_str_t* arr, size_t index)
{
    if(index >= arr->size)
        return;

    if(arr->ptr[index])
    {
        free(arr->ptr[index]);
        arr->ptr[index] = NULL;
    }
}
