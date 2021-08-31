#ifndef CLIPPING_HPP
#define CLIPPING_HPP

#include "vertex_shading.hpp"

#include "mesh.hpp"

struct ClipOut
{
    Mesh clippedMesh;
    const Vec4f *verticesHomogeneous;

    ClipOut(const Mesh &clippedMesh, const Vec4f *verticesHomogeneous);
};

ClipOut Clip(const Mesh &mesh, const VSOut *vsOut, const bool *trisToDraw);

#endif