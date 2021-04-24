#pragma once

#include <stdarg.h>

// Prompting function used internally by other prompting functions.
// It manages the capture of a (potentially empty) line and gets rid of the newline caracter.
char const* internal_prompt_string(
    int const count,
    char const* format,
    va_list args);

// Prints the prompt and re-prompts if the user just pressed 'enter'.
char const* prompt_string(
    int const count,
    char const* format,
    ...);

// Prints the proompt and if the user just pressed enter, returns the first argument in the va_list.
// (Fragile? Yes, it is. I agree.)
char const* prompt_optional_string(
    int const count,
    char const* format,
    ...);

// Uses prompt_string but returns only the first character.
char prompt_choice(
    char const* format,
    ...);

// An ersatz strcpy_s.
int strcpy_s(
    char *dest,
    int const count,
    const char* src);
