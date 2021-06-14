#ifndef VECTOR_3I_H
#define VECTOR_3I_H

#include "./types.h"

typedef struct Vector3i_t
{
    int_psp x;
    int_psp y;
    int_psp z;
} Vector3i;

void vector3i_print(const Vector3i *v);

#endif