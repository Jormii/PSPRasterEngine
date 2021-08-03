#ifndef CULLING_HPP
#define CULLING_HPP

#include <vector>

#include "vertex_shading.hpp"

#include "vec3i.hpp"

std::vector<Vec3i> BackfaceCulling(std::vector<Vec3i> triangles, std::vector<VSOut> vertexShadingOutput);

#endif