#include "./rgba.h"

#include <stdio.h>

void rgba_print(const RGBA *rgba)
{
    printf("(%u, %u, %u, %u)", rgba->r, rgba->g, rgba->b, rgba->a);
}