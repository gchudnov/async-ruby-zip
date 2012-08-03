#include "cerror.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

//
cerror_t* cerror_create(int code, const char* text)
{
    cerror_t* perr = (cerror_t*)malloc(sizeof(cerror_t));
    perr->code = code;

    if(text)
    {
        perr->message = (char*)malloc(strlen(text) + 1);
        strcpy(perr->message, text);
    }
    else
    {
        perr->message = NULL;
    }

    return perr;
}

//
void cerror_destroy(cerror_t* perr)
{
    if(perr)
    {
        if(perr->message)
        {
            free(perr->message);
            perr->message = NULL;
        }

        free(perr);
    }
}

//
int cerror_asprintf(char** ppasz, const char* format, ...)
{
    char buffer[384];
    int n;

    va_list ap;
    va_start(ap, format);

    n = vsprintf(buffer, format, ap);
    if(n)
    {
        *ppasz = strdup(buffer);
    }

    return n;
}


//
void cerror_free_message(cerror_t* perr)
{
    if(perr->message)
    {
        free(perr->message);
        perr->message = NULL;
    }
}

//
int cerror_is_error(cerror_t* perr)
{
    return (perr->code > 0 ? 1 : 0);
}
