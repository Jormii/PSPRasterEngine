#include "./utils.h"

#include <math.h>

Matrix4f look_at(const Vector3f *camera, const Vector3f *target, const Vector3f *up)
{
    Vector3f up_u = vector3f_normalized(up);

    Vector3f direction = vector3f_substract(target, camera);
    vector3f_normalize(&direction);

    Vector3f right = vector3f_cross(&up_u, &direction);

    // TODO: Optimize this part
    Matrix4f left_matrix = {
        {right.x, right.y, right.z, 0.0f},
        {up_u.x, up_u.y, up_u.z, 0.0f},
        {direction.x, direction.y, direction.z, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}};
    Matrix4f right_matrix = {
        {1.0f, 0.0f, 0.0f, -camera->x},
        {0.0f, 1.0f, 0.0f, -camera->y},
        {0.0f, 0.0f, 1.0f, -camera->z},
        {0.0f, 0.0f, 0.0f, 1.0f}};

    return matrix4f_matrix_mult(&left_matrix, &right_matrix);
}

Matrix4f perspective_fov(uint_psp width, uint_psp height, float_psp fov, float_psp near, float_psp far)
{
    float_psp aspect_ratio = ((float_psp)width) / ((float_psp)height);
    float_psp top = near * tanf(0.5f * fov);
    float_psp right = top * aspect_ratio;

    return perspective(-right, right, -top, top, near, far);
}

Matrix4f perspective(float_psp left, float_psp right, float_psp bottom, float_psp top, float_psp near, float_psp far)
{
    Matrix4f matrix;

    // First row
    matrix.r1.x = (2.0f * near) / (right - left);
    matrix.r1.y = 0.0f;
    matrix.r1.z = (right + left) / (right - left);
    matrix.r1.w = 0.0f;

    // Second row
    matrix.r2.x = 0.0f;
    matrix.r2.y = (2.0f * near) / (top - bottom);
    matrix.r2.z = (top + bottom) / (top - bottom);
    matrix.r2.w = 0.0f;

    // Third row
    matrix.r3.x = 0.0f;
    matrix.r3.y = 0.0f;
    matrix.r3.z = -(far + near) / (far - near);
    matrix.r3.w = -(2.0f * far * near) / (far - near);

    // Fourth row
    matrix.r4.x = 0.0f;
    matrix.r4.y = 0.0f;
    matrix.r4.z = -1.0f;
    matrix.r4.w = 0.0f;

    return matrix;
}

Matrix4f orthographic(float_psp left, float_psp right, float_psp bottom, float_psp top, float_psp near, float_psp far)
{
    Matrix4f matrix;

    // First row
    matrix.r1.x = 2.0f / (right - left);
    matrix.r1.y = 0.0f;
    matrix.r1.z = 0.0f;
    matrix.r1.w = -(right + left) / (right - left);

    // Second row
    matrix.r2.x = 0.0f;
    matrix.r2.y = 2.0f / (top - bottom);
    matrix.r2.z = 0.0f;
    matrix.r2.w = -(top + bottom) / (top - bottom);

    // Third row
    matrix.r3.x = 0.0f;
    matrix.r3.y = 0.0f;
    matrix.r3.z = -2.0f / (far - near);
    matrix.r3.w = -(far + near) / (far - near);

    // Fourth row
    matrix.r4.x = 0.0f;
    matrix.r4.y = 0.0f;
    matrix.r4.z = 0.0f;
    matrix.r4.w = 1.0f;

    return matrix;
}