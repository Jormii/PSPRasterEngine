#include "rasterization.hpp"

#include <cmath>

std::vector<Fragment> Rasterize(const Mesh &clippingOut, size_t width, size_t height)
{
    std::vector<Fragment> fragments;
    for (const Vec3i &tri : clippingOut.triangles)
    {
        size_t a{tri.x};
        size_t b{tri.y};
        size_t c{tri.z};

        std::vector<Fragment> triangleFragments{RasterizeTriangle(a, b, c, clippingOut, width, height)};
        fragments.insert(fragments.end(), triangleFragments.begin(), triangleFragments.end());
    }

    return fragments;
}

std::vector<Fragment> RasterizeTriangle(size_t a, size_t b, size_t c, const Mesh &mesh, size_t width, size_t height)
{
    Vec3f av{mesh.vertices[a]};
    Vec3f bv{mesh.vertices[b]};
    Vec3f cv{mesh.vertices[c]};

    // Bounding box
    float_t minX{std::min(av.x, std::min(bv.x, cv.x))};
    float_t maxX{std::max(av.x, std::max(bv.x, cv.x))};
    float_t minY{std::min(av.y, std::min(bv.y, cv.y))};
    float_t maxY{std::max(av.y, std::max(bv.y, cv.y))};

    Vec2i topLeftCoordinate{VertexPixel(Vec3f{minX, minY, 0.0f}, width, height)};
    Vec2i bottomRightCoordinate{VertexPixel(Vec3f{maxX, maxY, 0.0f}, width, height)};

    // Vertices pixels and center
    Vec2i aPixel{VertexPixel(av, width, height)};
    Vec2i bPixel{VertexPixel(bv, width, height)};
    Vec2i cPixel{VertexPixel(cv, width, height)};

    // Create fragments
    std::vector<Fragment> fragments;
    Vec2i pixel;
    for (int_psp x{topLeftCoordinate.x}; x <= bottomRightCoordinate.x; ++x)
    {
        for (int_psp y{topLeftCoordinate.y}; y <= bottomRightCoordinate.y; ++y)
        {
            pixel.x = x;
            pixel.y = y;

            Vec3f barycentricCoord{BarycentricCoordinates(pixel, aPixel, bPixel, cPixel)};
            bool inside{true};
            for (const float_t &value : {barycentricCoord.x, barycentricCoord.y, barycentricCoord.z})
            {
                if (value < 0.0f || value > 1.0f)
                {
                    inside = false;
                }
            }

            if (inside)
            {
                float_t depth{barycentricCoord.x * av.z + barycentricCoord.y * bv.z + barycentricCoord.z * cv.z};
                fragments.push_back(Fragment{x, y, depth});
            }
        }
    }

    return fragments;
}

Vec3f BarycentricCoordinates(const Vec2i &p, const Vec2i &a, const Vec2i &b, const Vec2i &c)
{
    Vec2i v0{b - a};
    Vec2i v1{c - a};
    Vec2i v2{p - a};

    float_t d00{static_cast<float_t>(Vec2i::Dot(v0, v0))};
    float_t d01{static_cast<float_t>(Vec2i::Dot(v0, v1))};
    float_t d11{static_cast<float_t>(Vec2i::Dot(v1, v1))};
    float_t d20{static_cast<float_t>(Vec2i::Dot(v2, v0))};
    float_t d21{static_cast<float_t>(Vec2i::Dot(v2, v1))};
    float_t denom = d00 * d11 - d01 * d01;

    float_t v{(d11 * d20 - d01 * d21) / denom};
    float_t w{(d00 * d21 - d01 * d20) / denom};
    return Vec3f{v, w, 1.0f - v - w};
}

Vec2i VertexPixel(const Vec3f &p, size_t width, size_t height)
{
    float_t widthF{static_cast<float_t>(width)};
    float_t heightF{static_cast<float_t>(height)};

    return Vec2i{
        static_cast<int_psp>(floorf(0.5f * (widthF - 1.0f) * (p.x + 1.0f))),
        static_cast<int_psp>(floorf(0.5f * (heightF - 1.0f) * (p.y + 1.0f)))};
}