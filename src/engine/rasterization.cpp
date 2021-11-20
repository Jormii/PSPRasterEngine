#include "rasterization.hpp"

#include <cmath>

#include "debug.hpp"
#include "constants.hpp"
#include "edge.hpp"

#include "vec2i.hpp"
#include "vec4i.hpp"

constexpr float_psp WIDTH_MINUS_1{PSP_WIDTH - 1.0f};
constexpr float_psp HEIGHT_MINUS_1{PSP_HEIGHT - 1.0f};

bool TriangleIsVisible(const Vec3i &tri, const BufferVertexData *buffer);
void RasterizeTriangle(const Vec3i &tri, const BufferVertexData *buffer, const Vec2f *screenSpace, std::vector<Fragment> &fragments, size_t width, size_t height);
Vec4i TriangleBBOX(const Vec2f &a, const Vec2f &b, const Vec2f &c);
bool PixelWithinTriangle(const Vec2f &pixel, const EdgeFunction *edgeFuncs);
void CreateFragment(const Vec3i &tri, const BufferVertexData *buffer, std::vector<Fragment> &fragments, const EdgeFunction *edgeFuncs, int_psp x, int_psp y, const Vec2f &pixel);
Vec3f BarycentricCoordinates(const Vec2f &pixel, const EdgeFunction *edgeFuncs);

bool TriangleIsVisible(const Vec3i &tri, const BufferVertexData *buffer)
{
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
    EdgeFunction edgeFuncs[]{
        EdgeFunction::FromPoints(screenSpace[tri.x], screenSpace[tri.y]),
        EdgeFunction::FromPoints(screenSpace[tri.y], screenSpace[tri.z]),
        EdgeFunction::FromPoints(screenSpace[tri.z], screenSpace[tri.x])};

    // Create fragments
    Vec4i bbox{TriangleBBOX(screenSpace[tri.x], screenSpace[tri.y], screenSpace[tri.z])};
    for (int_psp x{bbox.x}; x <= bbox.z; ++x)
    {
        for (int_psp y{bbox.y}; y <= bbox.w; ++y)
        {
            DebugStart(DebugIDs::CREATE_PIXEL);
            Vec2f pixel{
                static_cast<float_psp>(x) + 0.5f,
                static_cast<float_psp>(y) + 0.5f};
            DebugEnd(DebugIDs::CREATE_PIXEL);

            DebugStart(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_WITHIN_TRIANGLE);
            bool withinTri{PixelWithinTriangle(pixel, edgeFuncs)};
            DebugEnd(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_WITHIN_TRIANGLE);

            if (withinTri)
            {
                DebugStart(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_INTERPOLATION);
                CreateFragment(tri, buffer, fragments, edgeFuncs, x, y, pixel);
                DebugEnd(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_INTERPOLATION);
            }
        }
    }
}

Vec4i TriangleBBOX(const Vec2f &a, const Vec2f &b, const Vec2f &c)
{
    // Load vectors into VFPU
    asm(
        "lv.s S000, 0(%0);  lv.s S010, 4(%0);"
        "lv.s S001, 0(%1);  lv.s S011, 4(%1);"
        "lv.s S002, 0(%2);  lv.s S012, 4(%2);"
        :
        : "r"(&a), "r"(&b), "r"(&c)
        :);

    // Load screen bounds
    asm(
        "vzero.p R020;"
        "lv.s S021, 0(%0); lv.s S031, 0(%1);"
        :
        : "r"(&WIDTH_MINUS_1), "r"(&HEIGHT_MINUS_1)
        :);

    // Get BBOX
    // Lower bound
    asm(
        "vmin.p R003, R000, R001;"
        "vmin.p R003, R003, R002;"
        "vmax.p R003, R003, R020;");

    // Upper bound
    asm(
        "vmax.p R023, R000, R001;"
        "vmax.p R023, R023, R002;"
        "vmin.p R023, R023, R021;");

    // Store BBOX
    // TODO: Transform to Vec4i and use VFPU for casting
    Vec4i bbox;
    asm(
        "vf2in.q R003, R003, 0;"
        "sv.q R003, 0(%0);"
        :
        : "r"(&bbox)
        :);

    return bbox;
}

bool PixelWithinTriangle(const Vec2f &pixel, const EdgeFunction *edgeFuncs)
{
    for (size_t i{0}; i < 3; ++i)
    {
        float_psp fPixel{edgeFuncs[i].Evaluate(pixel)};
        bool firstCase{fPixel > 0.0f};
        bool secondCase{fPixel == 0.0f && edgeFuncs[i].a > 0.0f};
        bool thirdCase{fPixel == 0.0f && edgeFuncs[i].a == 0.0f && edgeFuncs[i].b >= 0.0f};
        if (!(firstCase || secondCase || thirdCase))
        {
            return false;
        }
    }

    return true;
}

void CreateFragment(const Vec3i &tri, const BufferVertexData *buffer, std::vector<Fragment> &fragments, const EdgeFunction *edgeFuncs, int_psp x, int_psp y, const Vec2f &pixel)
{
    // DebugStart(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_BARY_COORDS);
    Vec3f baryCoords{BarycentricCoordinates(pixel, edgeFuncs)};
    // DebugEnd(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_BARY_COORDS);

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
}

Vec3f BarycentricCoordinates(const Vec2f &pixel, const EdgeFunction *edgeFuncs)
{
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
    Vec2f *screenSpace{new Vec2f[mesh.vertexCount]};
    for (size_t i{0}; i < mesh.vertexCount; ++i)
    {
        screenSpace[i] = VertexScreenCoordinate(buffer[i].position);
    }

    // Generate fragments
    std::vector<Fragment> fragments;
    for (size_t i{0}; i < mesh.triangleCount; ++i)
    {
        const Vec3i &tri{mesh.triangles[i]};
        bool visible{TriangleIsVisible(tri, buffer)};

        if (visible)
        {
            RasterizeTriangle(tri, buffer, screenSpace, fragments);
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