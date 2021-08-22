#ifndef MESH_HPP
#define MESH_HPP

#include <iostream>
#include <vector>

#include "rgba.hpp"
#include "vec3i.hpp"
#include "vec3f.hpp"

struct Mesh
{
    std::vector<Vec3f> vertices;
    std::vector<Vec3i> triangles;
    std::vector<RGBA> color;

    Mesh();
    Mesh(std::vector<Vec3f> vertices, std::vector<Vec3i> triangles, std::vector<RGBA> color);
    Mesh(const Mesh &mesh);

    friend std::ostream &operator<<(std::ostream &out, const Mesh &mesh);
};

#endif