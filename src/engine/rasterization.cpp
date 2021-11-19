#include "rasterization.hpp"

#include <cmath>

#include "debug.hpp"
#include "constants.hpp"
#include "edge.hpp"

#include "vec2i.hpp"

bool TriangleIsVisible(const Vec3i &tri, const BufferVertexData *buffer);
void RasterizeTriangle(const Vec3i &tri, const BufferVertexData *buffer, const Vec2f *screenSpace, std::vector<Fragment> &fragments, size_t width, size_t height);
bool PixelWithinTriangle(const Vec2f &pixel, const EdgeFunction *edgeFuncs, int_psp topEdgeMask, int_psp leftEdgeMask);
Vec3f BarycentricCoordinates(const Vec2f &pixel, const Vec3i &tri, const BufferVertexData *buffer, const EdgeFunction *edgeFuncs);

bool TriangleIsVisible(const Vec3i &tri, const BufferVertexData *buffer)
{
    // Determine if backfacing
    const Vec3f &v1{buffer[tri.x].viewPos};
    const Vec3f &v2{buffer[tri.y].viewPos};
    const Vec3f &v3{buffer[tri.z].viewPos};

    Vec3f v1v2{v2 - v1};
    Vec3f v1v3{v3 - v1};
    Vec3f faceNormal{Vec3f::Cross(v1v2, v1v3)};

    float_psp dot{Vec3f::Dot(v1, faceNormal)};
    return dot <= 0.0f;
}

void RasterizeTriangle(const Vec3i &tri, const BufferVertexData *buffer, const Vec2f *screenSpace, std::vector<Fragment> &fragments)
{
    // Edge equations
    DebugStart(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_EDGE_FUNCS);
    EdgeFunction edgeFuncs[]{
        EdgeFunction::FromPoints(screenSpace[tri.x], screenSpace[tri.y]),
        EdgeFunction::FromPoints(screenSpace[tri.y], screenSpace[tri.z]),
        EdgeFunction::FromPoints(screenSpace[tri.z], screenSpace[tri.x])};
    DebugEnd(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_EDGE_FUNCS);

    // Retrieve top and left edges
    DebugStart(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_EDGES_MASKS);
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
    DebugEnd(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_EDGES_MASKS);

    // Bounding box
    DebugStart(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_BBOX);
    float_psp minX{std::min(screenSpace[tri.x].x, std::min(screenSpace[tri.y].x, screenSpace[tri.z].x))};
    float_psp minY{std::min(screenSpace[tri.x].y, std::min(screenSpace[tri.y].y, screenSpace[tri.z].y))};
    float_psp maxX{std::max(screenSpace[tri.x].x, std::max(screenSpace[tri.y].x, screenSpace[tri.z].x))};
    float_psp maxY{std::max(screenSpace[tri.x].y, std::max(screenSpace[tri.y].y, screenSpace[tri.z].y))};

    minX = std::max(minX, 0.0f);
    minY = std::max(minY, 0.0f);
    maxX = std::min(maxX, PSP_WIDTH - 1.0f);
    maxY = std::min(maxY, PSP_HEIGHT - 1.0f);

    Vec2i lowPixel{static_cast<int_psp>(floorf(minX)), static_cast<int_psp>(floorf(minY))};
    Vec2i highPixel{static_cast<int_psp>(floorf(maxX)), static_cast<int_psp>(floorf(maxY))};
    DebugEnd(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_BBOX);

    // Create fragments
    for (int_psp x{lowPixel.x}; x <= highPixel.x; ++x)
    {
        for (int_psp y{lowPixel.y}; y <= highPixel.y; ++y)
        {
            Vec2f pixelCenter{x + 0.5f, y + 0.5f};

            DebugStart(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_WITHIN_TRIANGLE);
            bool withinTri{PixelWithinTriangle(pixelCenter, edgeFuncs, topEdgeMask, leftEdgeMask)};
            DebugEnd(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_WITHIN_TRIANGLE);

            if (withinTri)
            {
                DebugStart(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_BARY_COORDS);
                Vec3f baryCoords{BarycentricCoordinates(pixelCenter, tri, buffer, edgeFuncs)};
                DebugEnd(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_BARY_COORDS);

                DebugStart(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_INTERPOLATION);
                float_psp depth{
                    baryCoords.x * buffer[tri.x].position.z +
                    baryCoords.y * buffer[tri.y].position.z +
                    baryCoords.z * buffer[tri.z].position.z};

                Vec3f viewPos{Vec3f::BarycentricInterpolation(
                    buffer[tri.x].viewPos,
                    buffer[tri.y].viewPos,
                    buffer[tri.z].viewPos,
                    baryCoords)};

                Vec3f normal{Vec3f::BarycentricInterpolation(
                    buffer[tri.x].normal,
                    buffer[tri.y].normal,
                    buffer[tri.z].normal,
                    baryCoords)};

                Vec4f color{Vec4f::BarycentricInterpolation(
                    buffer[tri.x].color,
                    buffer[tri.y].color,
                    buffer[tri.z].color,
                    baryCoords)};

                Vec2f uv{Vec2f::BarycentricInterpolation(
                    buffer[tri.x].uv,
                    buffer[tri.y].uv,
                    buffer[tri.z].uv,
                    baryCoords)};

                Fragment f{x, y, depth, viewPos, normal, color, uv};
                fragments.push_back(f);
                DebugEnd(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_INTERPOLATION);
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

Vec3f BarycentricCoordinates(const Vec2f &pixel, const Vec3i &tri, const BufferVertexData *buffer, const EdgeFunction *edgeFuncs)
{
    // TODO: Divide by w only when calculating UVs?
    // float_psp f0{edgeFuncs[0].Evaluate(pixel) / buffer[tri.x].positionHomo.w};
    // float_psp f1{edgeFuncs[1].Evaluate(pixel) / buffer[tri.y].positionHomo.w};
    // float_psp f2{edgeFuncs[2].Evaluate(pixel) / buffer[tri.z].positionHomo.w};
    float_psp f0{edgeFuncs[0].Evaluate(pixel)};
    float_psp f1{edgeFuncs[1].Evaluate(pixel)};
    float_psp f2{edgeFuncs[2].Evaluate(pixel)};
    float fSum{f0 + f1 + f2};

    float_psp u{f1 / fSum};
    float_psp v{f2 / fSum};
    float_psp w{1.0f - u - v};
    return Vec3f{u, v, w};
}

std::vector<Fragment> Rasterize(const Mesh &mesh, const BufferVertexData *buffer)
{
    // Calculate screen-space coordinates
    DebugStart(DebugIDs::RASTERIZATION_SCREEN_CORD);
    Vec2f *screenSpace{new Vec2f[mesh.vertexCount]};
    for (size_t i{0}; i < mesh.vertexCount; ++i)
    {
        screenSpace[i] = VertexScreenCoordinate(buffer[i].position);
    }
    DebugEnd(DebugIDs::RASTERIZATION_SCREEN_CORD);

    // Generate fragments
    std::vector<Fragment> fragments;
    for (size_t i{0}; i < mesh.triangleCount; ++i)
    {
        const Vec3i &tri{mesh.triangles[i]};
        DebugStart(DebugIDs::RASTERIZATION_TRIANGLE_VISIBILITY);
        bool visible{TriangleIsVisible(tri, buffer)};
        DebugEnd(DebugIDs::RASTERIZATION_TRIANGLE_VISIBILITY);

        if (visible)
        {
            DebugStart(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE);
            RasterizeTriangle(tri, buffer, screenSpace, fragments);
            DebugEnd(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE);
        }
    }

    return fragments;
}

Vec2f VertexScreenCoordinate(const Vec3f &p)
{
    return Vec2f{
        PSP_HALF_WIDTH * (p.x + 1.0f),
        PSP_HALF_HEIGHT * (p.y + 1.0f)};
}