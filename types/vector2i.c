#include "./vector2i.h"

int_psp vector2i_dot(const Vector2i *a, const Vector2i *b)
{
    return (a->x * b->x) + (a->x * b->y);
}

Vector2i vector2i_add(const Vector2i *a, const Vector2i *b)
{
    return (Vector2i){
        a->x + b->x,
        a->y + b->y};
}

Vector2i vector2i_substract(const Vector2i *a, const Vector2i *b)
{
    return (Vector2i){
        a->x - b->x,
        a->y - b->y};
}

Vector2i vector2i_divide(const Vector2i *v, int_psp scalar)
{
    return (Vector2i){
        result->x = v->x / scalar,
        result->y = v->y / scalar};
}