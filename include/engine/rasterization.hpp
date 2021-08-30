#ifndef RASTERIZATION_HPP
#define RASTERIZATION_HPP

#include <vector>

#include "mesh.hpp"
#include "fragment.hpp"

#include "vec2f.hpp"
#include "vec3f.hpp"

std::vector<Fragment> Rasterize(const Mesh &clippingOut, size_t width, size_t height);
std::vector<Fragment> RasterizeTriangle(int_psp a, int_psp b, int_psp c, const Mesh &mesh, const std::vector<Vec2f> screenSpaceCoords, size_t width, size_t height);
Vec2f VertexScreenCoordinate(const Vec3f &p, size_t width, size_t height);

#endif