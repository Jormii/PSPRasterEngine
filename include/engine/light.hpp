#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "vec3f.hpp"
#include "vec4f.hpp"

struct PointLight
{
    Vec3f position;
    float_psp r;
    Vec4f color;

    PointLight() = default;
    PointLight(const Vec3f &position, float_psp r, const Vec4f &color);
};

#endif