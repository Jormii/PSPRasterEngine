#ifndef MESH_HPP
#define MESH_HPP

#include <iostream>

#include "rgba.hpp"
#include "vec3i.hpp"
#include "vec3f.hpp"

struct VertexData
{
    Vec3f position;
    RGBA color;

    VertexData();
    VertexData(const Vec3f &position, const RGBA &color);
};

struct Mesh
{
    size_t vertexCount;
    size_t triangleCount;
    VertexData *vertexData;
    Vec3i *triangles;

    Mesh(size_t vertexCount, size_t triangleCount, const VertexData *vertexData, const Vec3i *triangles);

    friend std::ostream &operator<<(std::ostream &out, const Mesh &mesh);
};

#endif