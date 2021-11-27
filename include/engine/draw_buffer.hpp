#ifndef DRAWING_BUFFER_HPP
#define DRAWING_BUFFER_HPP

#include "vfpu.h"

#include "types.hpp"
#include "mesh.hpp"
#include "vec3f.hpp"
#include "vec4f.hpp"

struct BufferVertexData
{
    Vec3f position;
    Vec3f viewPos;
    Vec3f normal;
    Vec4f VFPU_ALIGN color;
    Vec2f uv;
};

#endif