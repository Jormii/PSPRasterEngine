#ifndef VECTOR_2I_H
#define VECTOR_2I_H

#include "./types.h"

typedef struct Vector2i_t
{
    int_psp x;
    int_psp y;
} Vector2i;

void vector2i_add(const Vector2i *a, const Vector2i *b, Vector2i *result);
void vector2i_substract(const Vector2i *a, const Vector2i *b, Vector2i *result);

void vector2i_divide(const Vector2i *v, int_psp scalar, Vector2i *result);

int_psp vector2i_dot(const Vector2i *a, const Vector2i *b);

#endif