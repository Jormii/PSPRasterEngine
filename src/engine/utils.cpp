#include "utils.hpp"

#include <cmath>

Mat4f TranslationMatrix(const Vec3f &p)
{
    return Mat4f(
        Vec4f{1.0f, 0.0f, 0.0f, p.x},
        Vec4f{0.0f, 1.0f, 0.0f, p.y},
        Vec4f{0.0f, 0.0f, 1.0f, p.z},
        Vec4f{0.0f, 0.0f, 0.0f, 1.0f});
}

Mat4f LookAt(const Vec3f &eye, const Vec3f &target, const Vec3f &up)
{
    Vec3f zAxis{(target - eye).Normalize()};
    Vec3f xAxis{Vec3f::Cross(zAxis, up).Normalize()};
    Vec3f yAxis{Vec3f::Cross(xAxis, zAxis)};

    zAxis = -zAxis;

    return Mat4f{
        Vec4f(xAxis, -Vec3f::Dot(xAxis, eye)),
        Vec4f(yAxis, -Vec3f::Dot(yAxis, eye)),
        Vec4f(zAxis, -Vec3f::Dot(zAxis, eye)),
        Vec4f(0.0f, 0.0f, 0.0f, 1.0f)};
}

Mat4f PerspectiveProj(float_psp left, float_psp right, float_psp bottom, float_psp top, float_psp near, float_psp far)
{
    // First row
    float_psp r1x(2.0f * near / (right - left));
    float_psp r1z((right + left) / (right - left));

    // Second row
    float_psp r2y(2.0f * near / (top - bottom));
    float_psp r2z((top + bottom) / (top - bottom));

    // Third row
    float_psp r3z(-(far + near) / (far - near));
    float_psp r3w(-(2.0f * far * near) / (far - near));

    return Mat4f(
        Vec4f(r1x, 0.0f, r1z, 0.0f),
        Vec4f(0.0f, r2y, r2z, 0.0f),
        Vec4f(0.0f, 0.0f, r3z, r3w),
        Vec4f(0.0f, 0.0f, -1.0f, 0.0f));
}

Mat4f PerspectiveProjFov(uint_psp width, uint_psp height, float_psp fov, float_psp near, float_psp far)
{
    float_psp wf(static_cast<float_psp>(width));
    float_psp hf(static_cast<float_psp>(height));
    float_psp aspectRatio(wf / hf);
    float_psp top = near * tanf(0.5f * fov);
    float_psp right = top * aspectRatio;

    return PerspectiveProj(-right, right, -top, top, near, far);
}

Mat4f OrthographicProj(float_psp left, float_psp right, float_psp bottom, float_psp top, float_psp near, float_psp far)
{
    // First row
    float_psp r1x(2.0f / (right - left));
    float_psp r1w(-(right + left) / (right - left));

    // Second row
    float_psp r2y(2.0f / (top - bottom));
    float_psp r2w(-(top + bottom) / (top - bottom));

    // Third row
    float_psp r3z(-2.0f / (far - near));
    float_psp r3w(-(far + near) / (far - near));

    return Mat4f(
        Vec4f(r1x, 0.0f, 0.0f, r1w),
        Vec4f(0.0f, r2y, 0.0f, r2w),
        Vec4f(0.0f, 0.0f, r3z, r3w),
        Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
}