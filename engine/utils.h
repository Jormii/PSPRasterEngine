#ifndef UTILS_H
#define UTILS_H

#include "../types/vector3f.h"
#include "../types/matrix4f.h"

void look_at(const Vector3f *camera, const Vector3f *target, const Vector3f *up, Matrix4f *out);
void perspective_fov(uint_psp width, uint_psp height, float_psp fov, float_psp near, float_psp far, Matrix4f *out);
void perspective(float_psp left, float_psp right, float_psp bottom, float_psp top, float_psp near, float_psp far, Matrix4f *out);
void orthographic(float_psp left, float_psp right, float_psp bottom, float_psp top, float_psp near, float_psp far, Matrix4f *out);

#endif