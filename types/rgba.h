#ifndef RGBA_H
#define RGBA_H

#include "./types.h"

typedef struct RGBA_t
{
    uint8_psp r;
    uint8_psp g;
    uint8_psp b;
    uint8_psp a;
} RGBA;

void rgba_print(const RGBA *rgba);

#endif