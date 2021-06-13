#include "./vector2i.h"

void vector2i_add(const Vector2i *a, const Vector2i *b, Vector2i *result)
{
    result->x = a->x + b->x;
    result->y = a->y + b->y;
}

void vector2i_substract(const Vector2i *a, const Vector2i *b, Vector2i *result)
{
    result->x = a->x - b->x;
    result->y = a->y - b->y;
}

void vector2i_divide(const Vector2i *v, int_psp scalar, Vector2i *result)
{
    result->x = v->x / scalar;
    result->y = v->y / scalar;
}

int_psp vector2i_dot(const Vector2i *a, const Vector2i *b)
{
    return (a->x * b->x) + (a->x * b->y);
}