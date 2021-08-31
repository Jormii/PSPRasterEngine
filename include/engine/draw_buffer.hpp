#ifndef DRAWING_BUFFER_HPP
#define DRAWING_BUFFER_HPP

#include "types.hpp"
#include "mesh.hpp"
#include "rgba.hpp"
#include "vec3f.hpp"
#include "vec4f.hpp"

struct BufferVertexData
{
    Vec3f position;
    Vec4f positionHomo;
    RGBA color;

    BufferVertexData();
};

#endif