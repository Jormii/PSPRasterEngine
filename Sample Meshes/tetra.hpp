#include <vector>

#include "vec3f.hpp"
#include "vec3i.hpp"
#include "rgba.hpp"
#include "mesh.hpp"

std::vector<Vec3f> tetraVertices{
    Vec3f{0.0f, -1.0f, -1.0f},
    Vec3f{0.866025f, -1.0f, 0.5f},
    Vec3f{-0.866025f, -1.0f, 0.5f},
    Vec3f{0.0f, 1.0f, 0.0f}};

std::vector<Vec3i> tetraTriangles{
    Vec3i{1, 2, 0},
    Vec3i{0, 3, 1},
    Vec3i{1, 3, 2},
    Vec3i{2, 3, 0}};

std::vector<RGBA> tetraColor{
    RGBA{255, 255, 255, 255},
    RGBA{255, 255, 255, 255},
    RGBA{255, 255, 255, 255},
    RGBA{255, 255, 255, 255}};

Mesh tetraMesh{tetraVertices, tetraTriangles, tetraColor};