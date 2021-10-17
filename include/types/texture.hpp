#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "types.hpp"
#include "vec2f.hpp"
#include "vec4f.hpp"

struct Texture
{
    size_t width;
    size_t height;
    Vec4f *map;

    Vec4f fetch(const Vec2f &uv) const;
};

#endif