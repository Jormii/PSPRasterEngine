#ifndef RASTERIZATION_HPP
#define RASTERIZATION_HPP

#include <vector>

#include "mesh.hpp"
#include "fragment.hpp"

#include "vec2i.hpp"
#include "vec3i.hpp"
#include "vec3f.hpp"

std::vector<Fragment> Rasterize(const Mesh &clippingOut, size_t width, size_t height);
std::vector<Fragment> RasterizeTriangle(int_psp a, int_psp b, int_psp c, const Mesh &mesh, size_t width, size_t height);
Vec3f BarycentricCoordinates(const Vec2i &p, const Vec2i &a, const Vec2i &b, const Vec2i &c);
Vec2i VertexPixel(const Vec3f &p, size_t width, size_t height);

#endif