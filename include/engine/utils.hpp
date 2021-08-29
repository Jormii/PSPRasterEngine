#ifndef UTILS_HPP
#define UTILS_HPP

#include "types.hpp"
#include "vec3f.hpp"
#include "mat4f.hpp"

Mat4f Translation(const Vec3f &p);
Mat4f Scaling(float_psp uniformScale);
Mat4f Scaling(const Vec3f &scale);

Mat4f LookAt(const Vec3f &eye, const Vec3f &target, const Vec3f &up);
Mat4f PerspectiveProj(float_psp left, float_psp right, float_psp bottom, float_psp top, float_psp near, float_psp far);
Mat4f PerspectiveProjFov(uint_psp width, uint_psp height, float_psp fov, float_psp near, float_psp far);
Mat4f OrthographicProj(float_psp left, float_psp right, float_psp bottom, float_psp top, float_psp near, float_psp far);

#endif