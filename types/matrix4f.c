#include "./matrix4f.h"

#include <stdio.h>

Matrix4f matrix4f_vector_mult(const Matrix4f *m, const Vector4f *v)
{
    return (Matrix4f){
        vector4f_dot(&m->r1, v),
        vector4f_dot(&m->r2, v),
        vector4f_dot(&m->r3, v),
        vector4f_dot(&m->r4, v)};
}

Matrix4f matrix4f_matrix_mult(const Matrix4f *a, const Matrix4f *b)
{
    Vector4f c1 = {b->r1.x, b->r2.x, b->r3.x, b->r4.x};
    Vector4f c2 = {b->r1.y, b->r2.y, b->r3.y, b->r4.y};
    Vector4f c3 = {b->r1.z, b->r2.z, b->r3.z, b->r4.z};
    Vector4f c4 = {b->r1.w, b->r2.w, b->r3.w, b->r4.w};

    Matrix4f result;

    // First row
    result.r1.x = vector4f_dot(&a->r1, &c1);
    result.r1.y = vector4f_dot(&a->r1, &c2);
    result.r1.z = vector4f_dot(&a->r1, &c3);
    result.r1.w = vector4f_dot(&a->r1, &c4);

    // Second row
    result.r2.x = vector4f_dot(&a->r2, &c1);
    result.r2.y = vector4f_dot(&a->r2, &c2);
    result.r2.z = vector4f_dot(&a->r2, &c3);
    result.r2.w = vector4f_dot(&a->r2, &c4);

    // Third row
    result.r3.x = vector4f_dot(&a->r3, &c1);
    result.r3.y = vector4f_dot(&a->r3, &c2);
    result.r3.z = vector4f_dot(&a->r3, &c3);
    result.r3.w = vector4f_dot(&a->r3, &c4);

    // Fourth row
    result.r4.x = vector4f_dot(&a->r4, &c1);
    result.r4.y = vector4f_dot(&a->r4, &c2);
    result.r4.z = vector4f_dot(&a->r4, &c3);
    result.r4.w = vector4f_dot(&a->r4, &c4);

    return result;
}

void matrix4f_print(const Matrix4f *m)
{
    printf("[");
    vector4f_print(&m->r1);
    printf("\n");
    vector4f_print(&m->r2);
    printf("\n");
    vector4f_print(&m->r3);
    printf("\n");
    vector4f_print(&m->r4);
    printf("]\n");
}