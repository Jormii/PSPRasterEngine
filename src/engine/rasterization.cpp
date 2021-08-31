#include "rasterization.hpp"

#include "edge.hpp"

#include "vec2i.hpp"

#include <cmath>

bool TriangleIsVisible(const Vec3i &tri, const BufferVertexData *buffer);
void RasterizeTriangle(const Vec3i &tri, const BufferVertexData *buffer, const Vec2f *screenSpace, std::vector<Fragment> &fragments, size_t width, size_t height);
bool PixelWithinTriangle(const Vec2f &pixel, const EdgeFunction *edgeFuncs, int_psp topEdgeMask, int_psp leftEdgeMask);
Vec3f BarycentricCoordinates(const Vec2f &pixel, const EdgeFunction *edgeFuncs);

bool TriangleIsVisible(const Vec3i &tri, const BufferVertexData *buffer)
{
    // Determine if backfacing
    const Vec3f &v1{buffer[tri.x].position};
    const Vec3f &v2{buffer[tri.y].position};
    const Vec3f &v3{buffer[tri.z].position};

    Vec3f v1v2{v2 - v1};
    Vec3f v2v3{v3 - v2};
    Vec3f faceNormal{Vec3f::Cross(v1, v2)};

    Vec3f centroid{(v1 + v2 + v3) / 3.0f}; // TODO: Unlikely unnecessary
    float_psp dot{Vec3f::Dot(centroid, faceNormal)};
    if (dot <= 0.0f)
    {
        return false;
    }

    // Determine if outside the frustum
    size_t insideCount{0};
    for (const Vec3f &v : {v1, v2, v3})
    {
        bool insideX{v.x >= -1.0f && v.x <= 1.0f};
        bool insideY{v.y >= -1.0f && v.y <= 1.0f};
        bool insideZ{v.z >= -1.0f && v.z <= 1.0f};

        insideCount += (insideX && insideY && insideZ);
    }

    return insideCount > 0;
}

void RasterizeTriangle(const Vec3i &tri, const BufferVertexData *buffer, const Vec2f *screenSpace, std::vector<Fragment> &fragments, size_t width, size_t height)
{
    // Edge equations
    EdgeFunction edgeFuncs[]{
        EdgeFunction::FromPoints(screenSpace[tri.x], screenSpace[tri.y]),
        EdgeFunction::FromPoints(screenSpace[tri.y], screenSpace[tri.z]),
        EdgeFunction::FromPoints(screenSpace[tri.z], screenSpace[tri.x])};

    // Retrieve top and left edges
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
    float_psp minX{std::min(screenSpace[tri.x].x, std::min(screenSpace[tri.y].x, screenSpace[tri.z].x))};
    float_psp minY{std::min(screenSpace[tri.x].y, std::min(screenSpace[tri.y].y, screenSpace[tri.z].y))};
    float_psp maxX{std::max(screenSpace[tri.x].x, std::max(screenSpace[tri.y].x, screenSpace[tri.z].x))};
    float_psp maxY{std::max(screenSpace[tri.x].y, std::max(screenSpace[tri.y].y, screenSpace[tri.z].y))};

    Vec2i lowPixel{static_cast<int_psp>(floorf(minX)), static_cast<int_psp>(floorf(minY))};
    Vec2i highPixel{static_cast<int_psp>(floorf(maxX)), static_cast<int_psp>(floorf(maxY))};

    // Create fragments
    for (int_psp x{lowPixel.x}; x <= highPixel.x; ++x)
    {
        for (int_psp y{lowPixel.y}; y <= highPixel.y; ++y)
        {
            Vec2f pixelCenter{x + 0.5f, y + 0.5f};
            if (PixelWithinTriangle(pixelCenter, edgeFuncs, topEdgeMask, leftEdgeMask))
            {
                Vec3f baryCoords{BarycentricCoordinates(pixelCenter, edgeFuncs)};

                float_psp depth{
                    baryCoords.x * buffer[tri.x].position.z +
                    baryCoords.y * buffer[tri.y].position.z +
                    baryCoords.z * buffer[tri.z].position.z};

                // TODO: Interpolate color
                RGBA white{255, 255, 255, 255};

                Fragment f{x, y, depth, white};
                fragments.push_back(f);
            }
        }
    }
}

bool PixelWithinTriangle(const Vec2f &pixel, const EdgeFunction *edgeFuncs, int_psp topEdgeMask, int_psp leftEdgeMask)
{
    for (size_t i{0}; i < 3; ++i)
    {
        float_psp fPixel{edgeFuncs[i].Evaluate(pixel)};
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

Vec3f BarycentricCoordinates(const Vec2f &pixel, const EdgeFunction *edgeFuncs)
{
    float_psp f0{edgeFuncs[0].Evaluate(pixel)};
    float_psp f1{edgeFuncs[1].Evaluate(pixel)};
    float_psp f2{edgeFuncs[2].Evaluate(pixel)};
    float fSum{f0 + f1 + f2};

    float_psp v{f0 / fSum};
    float_psp w{f1 / fSum};
    return Vec3f{1.0f - v - w, v, w};
}

std::vector<Fragment> Rasterize(const Mesh &mesh, const BufferVertexData *buffer, size_t width, size_t height)
{
    // Calculate screen-space coordinates
    Vec2f *screenSpace{new Vec2f[mesh.vertexCount]};
    for (size_t i{0}; i < mesh.vertexCount; ++i)
    {
        screenSpace[i] = VertexScreenCoordinate(buffer[i].position, width, height);
    }

    // Generate fragments
    std::vector<Fragment> fragments;
    for (size_t i{0}; i < mesh.triangleCount; ++i)
    {
        const Vec3i &tri{mesh.triangles[i]};
        if (!TriangleIsVisible(tri, buffer))
        {
            continue;
        }

        RasterizeTriangle(tri, buffer, screenSpace, fragments, width, height);
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