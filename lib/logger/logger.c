#include "logger/logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

FILE *file;

void lopen(char const* chemin)
{
    if(!file)
    {
        file = fopen(chemin, "a");
    }
}

void llog(char const* format, ...)
{
    if(!file)
    {
        return;
    }
    
    char time_s[100];
    time_t now = time(0);
    strftime(time_s, 100, "%Y-%m-%d %H:%M:%S.000", localtime(&now));
    fprintf(file, "[%s] ", time_s);

    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    fprintf(file, "\n");
    fflush(file);
}

void lclose()
{
    fclose(file);
    file = NULL;
}
