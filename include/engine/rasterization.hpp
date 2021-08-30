#ifndef RASTERIZATION_HPP
#define RASTERIZATION_HPP

#include <vector>

#include "mesh.hpp"
#include "fragment.hpp"

#include "vec2i.hpp"
#include "vec2f.hpp"
#include "vec3i.hpp"
#include "vec3f.hpp"

struct EdgeFunction
{
    // Edge function: e(x, y): ax + by + c = 0
    float_psp a;
    float_psp b;
    float_psp c;

    EdgeFunction(float_psp a, float_psp b, float_psp c);

    float_psp Evaluate(const Vec2f &p) const;

    static EdgeFunction FromPoints(const Vec2f &p, const Vec2f &q);
};

std::vector<Fragment> Rasterize(const Mesh &clippingOut, size_t width, size_t height);
std::vector<Fragment> RasterizeTriangle(int_psp a, int_psp b, int_psp c, const Mesh &mesh, const std::vector<Vec2f> screenSpaceCoords, size_t width, size_t height);
Vec3f BarycentricCoordinates(const Vec2f &p, const Vec2f &a, const Vec2f &b, const Vec2f &c);
Vec2f VertexScreenCoordinate(const Vec3f &p, size_t width, size_t height);
Vec2i VertexPixel(const Vec3f &p, size_t width, size_t height);

#endif