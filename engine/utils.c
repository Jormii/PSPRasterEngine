#include "./utils.h"

#include <math.h>

void look_at(const Vector3f *camera, const Vector3f *target, const Vector3f *up, Matrix4f *out)
{
    Vector3f direction;
    vector3f_substract(target, camera, &direction);
    vector3f_normalize(&direction);

    Vector3f right;
    vector3f_cross(up, &direction, &right);
    vector3f_normalize(&right);

    // TODO: Optimize this part
    Matrix4f left_matrix = {
        {right.x, right.y, right.z, 0.0f},
        {up->x, up->y, up->z, 0.0f},
        {direction.x, direction.y, direction.z, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}};
    Matrix4f right_matrix = {
        {1.0f, 0.0f, 0.0f, -camera->x},
        {0.0f, 1.0f, 0.0f, -camera->y},
        {0.0f, 0.0f, 1.0f, -camera->z},
        {0.0f, 0.0f, 0.0f, 1.0f}};

    matrix4f_matrix_mult(&left_matrix, &right_matrix, out);
}

void perspective_fov(uint_psp width, uint_psp height, float_psp fov, float_psp near, float_psp far, Matrix4f *out)
{
    float_psp aspect_ratio = ((float_psp)width) / ((float_psp)height);
    float_psp top = near * tanf(0.5f * fov);
    float_psp right = top * aspect_ratio;

    perspective(-right, right, -top, top, near, far, out);
}

void perspective(float_psp left, float_psp right, float_psp bottom, float_psp top, float_psp near, float_psp far, Matrix4f *out)
{
    // First row
    out->r1.x = (2.0f * near) / (right - left);
    out->r1.y = 0.0f;
    out->r1.z = (right + left) / (right - left);
    out->r1.w = 0.0f;

    // Second row
    out->r2.x = 0.0f;
    out->r2.y = (2.0f * near) / (top - bottom);
    out->r2.z = (top + bottom) / (top - bottom);
    out->r2.w = 0.0f;

    // Third row
    out->r3.x = 0.0f;
    out->r3.y = 0.0f;
    out->r3.z = -(far + near) / (far - near);
    out->r3.w = -(2.0f * far * near) / (far - near);

    // Fourth row
    out->r4.x = 0.0f;
    out->r4.y = 0.0f;
    out->r4.z = -1.0f;
    out->r4.w = 0.0f;
}

void orthographic(float_psp left, float_psp right, float_psp bottom, float_psp top, float_psp near, float_psp far, Matrix4f *out)
{
    // First row
    out->r1.x = 2.0f / (right - left);
    out->r1.y = 0.0f;
    out->r1.z = 0.0f;
    out->r1.w = -(right + left) / (right - left);

    // Second row
    out->r2.x = 0.0f;
    out->r2.y = 2.0f / (top - bottom);
    out->r2.z = 0.0f;
    out->r2.w = -(top + bottom) / (top - bottom);

    // Third row
    out->r3.x = 0.0f;
    out->r3.y = 0.0f;
    out->r3.z = -2.0f / (far - near);
    out->r3.w = -(far + near) / (far - near);

    // Fourth row
    out->r4.x = 0.0f;
    out->r4.y = 0.0f;
    out->r4.z = 0.0f;
    out->r4.w = 1.0f;
}