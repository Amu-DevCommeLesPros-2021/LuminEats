#include "prompt.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#if defined(__linux__)
    #include <stdio_ext.h>
#endif
#include <string.h>

#define TAILLE_SAISIE 256

int strcpy_s(
    char *dest,
    int const count,
    const char* src)
{
    if(count <= 1 || !dest)
    {
        if(!dest || !count)
        {
            return 0;
        }
        *dest = '\0';
        return 0;
    }
    
    for(char* d = dest + count - 1; (dest != d || (*dest = '\0')) && (*dest = *src); ++dest, ++src);
    
    return 0;
}

char const* internal_prompt_string(
    int const count,
    char const* format,
    va_list args)
{
    static char buffer[TAILLE_SAISIE];
    assert(count < TAILLE_SAISIE);

    vprintf(format, args);
    fflush(stdout);

    fgets(buffer, count + 1, stdin);

    #if defined(__linux__)
        __fpurge(stdin);
    #else
        fpurge(stdin);
    #endif

    // Replace the ending '\n' by '\0'.
    char *c = strrchr(buffer, '\n');
    if(c)
    {
        *c = '\0';
    }

    return buffer;
}

char const* prompt_string(
    int const count,
    char const* format,
    ...)
{
    char const* saisie = NULL;
    do
    {
        va_list args;
        va_start(args, format);
        saisie = internal_prompt_string(count, format, args);
        va_end(args);
    }
    while(strlen(saisie) == 0);

    return saisie;
}

char const* prompt_optional_string(
    int const count,
    char const* format,
    ...)
{
    static char buffer[TAILLE_SAISIE];
    va_list args;
    
    {
        va_start(args, format);
        strcpy(buffer, internal_prompt_string(count, format, args));
        va_end(args);
    }

    // If the user simply "newlined" the prompt away, we'll return the first argument.
    if(strlen(buffer) == 0)
    {
        va_start(args, format);
        strcpy(buffer, va_arg(args, char*));
        va_end(args);
    }

    return buffer;
}

char prompt_choice(
    char const* format,
    ...)
{
    va_list args;
    va_start(args, format);
    char const c = prompt_string(1, format, args)[0];
    va_end(args);

    return c;
}
