#ifndef TETRA_HPP
#define TETRA_HPP

#include "mesh.hpp"

RGBA white{255, 255, 255, 255};

size_t tetraVertexCount{4};
size_t tetraTriangleCount{4};

VertexData tetraVertexData[]{
    VertexData{Vec3f{0.0f, -1.0f, -1.0f}, white},
    VertexData{Vec3f{0.866025f, -1.0f, 0.5f}, white},
    VertexData{Vec3f{-0.866025f, -1.0f, 0.5f}, white},
    VertexData{Vec3f{0.0f, 1.0f, 0.0f}, white}};

Vec3i tetraTriangles[]{
    Vec3i{1, 2, 0},
    Vec3i{0, 3, 1},
    Vec3i{1, 3, 2},
    Vec3i{2, 3, 0}};

Mesh tetraMesh{tetraVertexCount, tetraTriangleCount, tetraVertexData, tetraTriangles};

#endif