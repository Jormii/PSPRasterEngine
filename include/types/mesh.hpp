#ifndef MESH_HPP
#define MESH_HPP

#include <iostream>

#include "vec2f.hpp"
#include "vec3i.hpp"
#include "vec3f.hpp"
#include "vec4f.hpp"

struct VertexData
{
    Vec3f position;
    Vec3f normal;
    Vec4f color;
    Vec2f uv;

    VertexData();
    VertexData(const Vec3f &position, const Vec3f &normal, const Vec4f &color, const Vec2f &uv);
};

struct Mesh
{
    size_t vertexCount;
    size_t triangleCount;
    VertexData *vertexData;
    Vec3i *triangles;

    Mesh(size_t vertexCount, size_t triangleCount, VertexData *vertexData, Vec3i *triangles);

    friend std::ostream &operator<<(std::ostream &out, const Mesh &mesh);
};

#endif