#include "rasterization.hpp"

#include "vec2f.hpp"

#include <cmath>

bool PixelWithinTriangle(const Vec2f &pixel, const EdgeFunction *edgeFuncs, int_psp topEdgeMask, int_psp leftEdgeMask);

EdgeFunction::EdgeFunction(float_psp a, float_psp b, float_psp c) : a{a}, b{b}, c{c}
{
}

float_psp EdgeFunction::Evaluate(const Vec2f &p) const
{
    return a * p.x + b * p.y + c;
}

EdgeFunction EdgeFunction::FromPoints(const Vec2f &p, const Vec2f &q)
{
    Vec2f v{q - p};
    return EdgeFunction{
        -v.y,
        v.x,
        v.y * p.x - v.x * p.y};
}

std::vector<Fragment> Rasterize(const Mesh &clippingOut, size_t width, size_t height)
{
    // Calculate screen-space coordinates
    std::vector<Vec2f> screenSpace;
    for (const Vec3f &v : clippingOut.vertices)
    {
        screenSpace.push_back(VertexScreenCoordinate(v, width, height));
    }

    // Generate fragments
    std::vector<Fragment> fragments;
    for (const Vec3i &tri : clippingOut.triangles)
    {
        int_psp a{tri.x};
        int_psp b{tri.y};
        int_psp c{tri.z};

        std::vector<Fragment> triangleFragments{RasterizeTriangle(a, b, c, clippingOut, screenSpace, width, height)};
        fragments.insert(fragments.end(), triangleFragments.begin(), triangleFragments.end());
    }

    return fragments;
}

std::vector<Fragment> RasterizeTriangle(int_psp a, int_psp b, int_psp c, const Mesh &mesh, const std::vector<Vec2f> screenSpaceCoords, size_t width, size_t height)
{
    // Edge equations
    EdgeFunction edgeFuncs[]{
        EdgeFunction::FromPoints(screenSpaceCoords[a], screenSpaceCoords[b]),
        EdgeFunction::FromPoints(screenSpaceCoords[b], screenSpaceCoords[c]),
        EdgeFunction::FromPoints(screenSpaceCoords[c], screenSpaceCoords[a])};

    // Check top and left edges
    int_psp topEdgeMask{0};
    int_psp leftEdgeMask{0};
    for (size_t i{0}; i < 3; ++i)
    {
        if (edgeFuncs[i].a > 0 && edgeFuncs[i].b == 0)
        {
            topEdgeMask |= 1 << i;
        }

        if (edgeFuncs[i].a > 0)
        {
            leftEdgeMask |= 1 << i;
        }
    }

    // Bounding box
    float_psp minX{std::min(screenSpaceCoords[a].x, std::min(screenSpaceCoords[b].x, screenSpaceCoords[c].x))};
    float_psp minY{std::min(screenSpaceCoords[a].y, std::min(screenSpaceCoords[b].y, screenSpaceCoords[c].y))};
    float_psp maxX{std::max(screenSpaceCoords[a].x, std::max(screenSpaceCoords[b].x, screenSpaceCoords[c].x))};
    float_psp maxY{std::max(screenSpaceCoords[a].y, std::max(screenSpaceCoords[b].y, screenSpaceCoords[c].y))};

    Vec2i lowPixel{static_cast<int_psp>(floorf(minX)), static_cast<int_psp>(floorf(minY))};
    Vec2i highPixel{static_cast<int_psp>(floorf(maxX)), static_cast<int_psp>(floorf(maxY))};

    // Create fragments
    std::vector<Fragment> fragments;
    for (int_psp x{lowPixel.x}; x <= highPixel.x; ++x)
    {
        for (int_psp y{lowPixel.y}; y <= highPixel.y; ++y)
        {
            Vec2f pixelCenter{x + 0.5f, y + 0.5f};
            if (PixelWithinTriangle(pixelCenter, edgeFuncs, topEdgeMask, leftEdgeMask))
            {
                Vec3f baryCoords{BarycentricCoordinates(pixelCenter, screenSpaceCoords[a], screenSpaceCoords[b], screenSpaceCoords[c])};

                float_psp depth{
                    baryCoords.x * mesh.vertices[a].z +
                    baryCoords.y * mesh.vertices[b].z +
                    baryCoords.z * mesh.vertices[c].z};
                Fragment f{x, y, depth};
                fragments.push_back(f);
            }
        }
    }

    return fragments;
}

Vec3f BarycentricCoordinates(const Vec2f &p, const Vec2f &a, const Vec2f &b, const Vec2f &c)
{
    Vec2f v0{b - a};
    Vec2f v1{c - a};
    Vec2f v2{p - a};

    float_t d00{static_cast<float_t>(Vec2f::Dot(v0, v0))};
    float_t d01{static_cast<float_t>(Vec2f::Dot(v0, v1))};
    float_t d11{static_cast<float_t>(Vec2f::Dot(v1, v1))};
    float_t d20{static_cast<float_t>(Vec2f::Dot(v2, v0))};
    float_t d21{static_cast<float_t>(Vec2f::Dot(v2, v1))};
    float_t denom = d00 * d11 - d01 * d01;

    float_t v{(d11 * d20 - d01 * d21) / denom};
    float_t w{(d00 * d21 - d01 * d20) / denom};
    return Vec3f{v, w, 1.0f - v - w};
}

Vec2f VertexScreenCoordinate(const Vec3f &p, size_t width, size_t height)
{
    float_psp wHalf{0.5f * width};
    float_psp hHalf{0.5f * height};

    return Vec2f{
        wHalf * (p.x + 1.0f),
        hHalf * (p.y + 1.0f)};
}

Vec2i VertexPixel(const Vec3f &p, size_t width, size_t height)
{
    float_t widthF{static_cast<float_t>(width)};
    float_t heightF{static_cast<float_t>(height)};

    return Vec2i{
        static_cast<int_psp>(floorf(0.5f * (widthF - 1.0f) * (p.x + 1.0f))),
        static_cast<int_psp>(floorf(0.5f * (heightF - 1.0f) * (p.y + 1.0f)))};
}

bool PixelWithinTriangle(const Vec2f &pixel, const EdgeFunction *edgeFuncs, int_psp topEdgeMask, int_psp leftEdgeMask)
{
    for (size_t i{0}; i < 3; ++i)
    {
        // TODO: This minus is wrong
        float_psp fPixel{-edgeFuncs[i].Evaluate(pixel)};
        if (fPixel < 0.0f)
        {
            return false;
        }

        bool isTopEdge{static_cast<bool>(topEdgeMask & (1 << i))};
        bool isLeftEdge{static_cast<bool>(leftEdgeMask & (1 << i))};
        if (fPixel == 0.0f && !(isTopEdge || isLeftEdge))
        {
            return false;
        }
    }

    return true;
}