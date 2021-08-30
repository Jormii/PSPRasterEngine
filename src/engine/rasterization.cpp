#include "rasterization.hpp"

#include "vec2i.hpp"

#include <cmath>

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

bool PixelWithinTriangle(const Vec2f &pixel, const EdgeFunction *edgeFuncs, int_psp topEdgeMask, int_psp leftEdgeMask);
Vec3f BarycentricCoordinates(const Vec2f &pixel, const EdgeFunction *edgeFuncs);
RGBA InterpolateColor(const Vec3f &weights, const RGBA &c1, const RGBA &c2, const RGBA &c3);

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

Vec3f BarycentricCoordinates(const Vec2f &p, EdgeFunction *edgeFuncs)
{
    float_psp f0{edgeFuncs[0].Evaluate(p)};
    float_psp f1{edgeFuncs[1].Evaluate(p)};
    float_psp f2{edgeFuncs[2].Evaluate(p)};
    float fSum{f0 + f1 + f2};

    float_psp v{f0 / fSum};
    float_psp w{f1 / fSum};
    return Vec3f{1.0f - v - w, v, w};
}

RGBA InterpolateColor(const Vec3f &weights, const RGBA &c1, const RGBA &c2, const RGBA &c3)
{
    // Red
    Vec3f R{static_cast<float_psp>(c1.r), static_cast<float_psp>(c2.r), static_cast<float_psp>(c3.r)};
    float_psp r{floorf(weights.x * R.x + weights.y * R.y + weights.z * R.z)};

    // Green
    Vec3f G{static_cast<float_psp>(c1.g), static_cast<float_psp>(c2.g), static_cast<float_psp>(c3.g)};
    float_psp g{floorf(weights.x * G.x + weights.y * G.y + weights.z * G.z)};

    // Blue
    Vec3f B{static_cast<float_psp>(c1.b), static_cast<float_psp>(c2.b), static_cast<float_psp>(c3.b)};
    float_psp b{floorf(weights.x * B.x + weights.y * B.y + weights.z * B.z)};

    // Opacity
    Vec3f O{static_cast<float_psp>(c1.opacity), static_cast<float_psp>(c2.opacity), static_cast<float_psp>(c3.opacity)};
    float_psp opacity{floorf(weights.x * O.x + weights.y * O.y + weights.z * O.z)};

    std::cout << r << "\n";
    std::cout << g << "\n";
    std::cout << b << "\n";
    std::cout << opacity << "\n";

    return RGBA{
        static_cast<uint8_psp>(r),
        static_cast<uint8_psp>(g),
        static_cast<uint8_psp>(b),
        static_cast<uint8_psp>(opacity)};
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
                Vec3f baryCoords{BarycentricCoordinates(pixelCenter, edgeFuncs)};

                float_psp depth{
                    baryCoords.x * mesh.vertices[a].z +
                    baryCoords.y * mesh.vertices[b].z +
                    baryCoords.z * mesh.vertices[c].z};
                // RGBA color{InterpolateColor(baryCoords, mesh.color[a], mesh.color[b], mesh.color[c])};
                RGBA color{255, 255, 255, 255};

                Fragment f{x, y, depth, color};
                fragments.push_back(f);
            }
        }
    }

    return fragments;
}

Vec2f VertexScreenCoordinate(const Vec3f &p, size_t width, size_t height)
{
    float_psp wHalf{0.5f * width};
    float_psp hHalf{0.5f * height};

    return Vec2f{
        wHalf * (p.x + 1.0f),
        hHalf * (p.y + 1.0f)};
}