#include "texture.hpp"

#include <cmath>

Vec4f Texture::fetch(const Vec2f &uv) const
{
    size_t uIndex{static_cast<size_t>(floorl(uv.x * width))};
    size_t vIndex{static_cast<size_t>(floorl(uv.y * height))};

    return map[width * vIndex + uIndex];
}