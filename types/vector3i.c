#include "./vector3i.h"

#include <stdio.h>

void vector3i_print(const Vector3i *v)
{
    printf("(%d, %d, %d)", v->x, v->y, v->z);
}