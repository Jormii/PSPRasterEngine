#ifndef TETRA_HPP
#define TETRA_HPP

#include "mesh.hpp"

size_t tetraVertexCount{4};
size_t tetraTriangleCount{4};

VertexData tetraVertexData[]{
    VertexData{Vec3f{0.0f, -1.0f, -1.0f}, RGBA{255, 0, 0}},
    VertexData{Vec3f{0.866025f, -1.0f, 0.5f}, RGBA{0, 255, 0}},
    VertexData{Vec3f{-0.866025f, -1.0f, 0.5f}, RGBA{0, 0, 255}},
    VertexData{Vec3f{0.0f, 1.0f, 0.0f}, RGBA{255, 255, 255}}};

Vec3i tetraTriangles[]{
    Vec3i{1, 2, 0},
    Vec3i{0, 3, 1},
    Vec3i{1, 3, 2},
    Vec3i{2, 3, 0}};

Mesh tetraMesh{tetraVertexCount, tetraTriangleCount, tetraVertexData, tetraTriangles};

#endif