#include "rasterization.hpp"

#include <cmath>

#include "vfpu.hpp"
#include "debug.hpp"
#include "constants.hpp"
#include "edge.hpp"

#include "vec2i.hpp"
#include "vec4i.hpp"

#define GRID_SIZE 2
#define GRID_CELLS (GRID_SIZE * GRID_SIZE)

const Vec4f __attribute__((aligned(16))) ZERO_TO_THREE{0.0f, 1.0f, 2.0f, 3.0f};

constexpr float_psp VFPU_ALIGN GRID_SIZE_VEC4[]{GRID_SIZE, GRID_SIZE, GRID_SIZE, GRID_SIZE};

constexpr float_psp HALF_SCREEN_VEC2[]{
    0.5f * static_cast<float_psp>(PSP_WIDTH),
    0.5f * static_cast<float_psp>(PSP_HEIGHT)};

constexpr float_psp MAX_SCREEN_VEC2[]{
    static_cast<float_psp>(PSP_WIDTH - 1.0f),
    static_cast<float_psp>(PSP_HEIGHT - 1.0f)};

bool TriangleIsVisible(const Vec3i &tri, const BufferVertexData *buffer);
void RasterizeTriangle(const Vec3i &tri, const BufferVertexData *buffer, std::vector<Fragment> &fragments);
void UploadScreenCoordinatesToVFPU(const Vec3i &tri, const BufferVertexData *buffer);
void InitializeEdgeFunctions();
Vec4i TriangleBBOX();

void CalculateBarycentricCoordinates();
bool PixelWithinTriangle(const Vec2f &pixel, const EdgeFunction *edgeFuncs);
void CreateFragment(const Vec3i &tri, const BufferVertexData *buffer, std::vector<Fragment> &fragments, const EdgeFunction *edgeFuncs, int_psp x, int_psp y, const Vec2f &pixel);
Vec3f BarycentricCoordinates(const Vec2f &pixel, const EdgeFunction *edgeFuncs);

bool TriangleIsVisible(const Vec3i &tri, const BufferVertexData *buffer)
{
    // TODO: Improve by using the VFPU
    const Vec3f &v1{buffer[tri.x].viewPos};
    const Vec3f &v2{buffer[tri.y].viewPos};
    const Vec3f &v3{buffer[tri.z].viewPos};

    Vec3f v1v2{v2 - v1};
    Vec3f v1v3{v3 - v1};
    Vec3f faceNormal{Vec3f::Cross(v1v2, v1v3)};

    float_psp dot{Vec3f::Dot(v1, faceNormal)};
    return dot <= 0.0f;
}

void RasterizeTriangle(const Vec3i &tri, const BufferVertexData *buffer, std::vector<Fragment> &fragments)
{
    DebugStart(DebugIDs::UPLOAD_COORDINATES);
    UploadScreenCoordinatesToVFPU(tri, buffer);
    DebugEnd(DebugIDs::UPLOAD_COORDINATES);

    DebugStart(DebugIDs::INIT_EDGES);
    InitializeEdgeFunctions();
    DebugEnd(DebugIDs::INIT_EDGES);

    DebugStart(DebugIDs::BBOX);
    Vec4i bbox{TriangleBBOX()};
    DebugEnd(DebugIDs::BBOX);
    for (int_psp y{bbox.y}; y <= bbox.w; y += GRID_SIZE)
    {
        for (int_psp x{bbox.x}; x <= bbox.z; x += GRID_SIZE)
        {
            Vec2f pixel{
                static_cast<float_psp>(x) + 0.5f,
                static_cast<float_psp>(y) + 0.5f};
        }
    }
#if 0
    // Edge equations
    EdgeFunction edges[]{
        EdgeFunction::FromPoints(screenSpace[tri.x], screenSpace[tri.y]),
        EdgeFunction::FromPoints(screenSpace[tri.y], screenSpace[tri.z]),
        EdgeFunction::FromPoints(screenSpace[tri.z], screenSpace[tri.x])};

    // Create fragments
    Vec4i bbox{TriangleBBOX(screenSpace[tri.x], screenSpace[tri.y], screenSpace[tri.z])};

    float_psp startY{static_cast<float_psp>(bbox.y) + 0.5f};
    Vec2f pixel{
        static_cast<float_psp>(bbox.x) + 0.5f,
        startY};

    for (int_psp x{bbox.x}; x <= bbox.z; x += GRID_SIZE)
    {
        for (int_psp y{bbox.y}; y <= bbox.w; y += GRID_SIZE)
        {
            CalculateEdgeFunctions(pixel, edges);
            CalculateBarycentricCoordinates();

            // TODO: Not correct. Temporal.
            CreateFragment(tri, buffer, fragments, edges, x, y, pixel);
        }
        pixel.y += static_cast<float_psp>(GRID_SIZE);
    }

    pixel.x += static_cast<float_psp>(GRID_SIZE);
    pixel.y = startY;
#endif
}

void UploadScreenCoordinatesToVFPU(const Vec3i &tri, const BufferVertexData *buffer)
{
    LOAD_VEC2_ROW_L(7, 0, &(buffer[tri.x].position));
    LOAD_VEC2_ROW_L(7, 1, &(buffer[tri.y].position));
    LOAD_VEC2_ROW_L(7, 2, &(buffer[tri.z].position));

    asm(
        // Add 1
        "vadd.t C100, C700, C030;"
        "vadd.t C110, C710, C030;"

        // Multiply by half width/height
        "vmul.t C100, C100, C000;"
        "vmul.t C110, C110, C010;");
}

void InitializeEdgeFunctions()
{
    // Calculate vectors => Parameters b, -a
    // b in C200, -a in C210
    asm(
        "vsub.p R200, R101, R100;" // A -> B
        "vsub.p R201, R102, R101;" // B -> C
        "vsub.p R202, R100, R102;" // C -> A
    );

    // Calculate c
    asm(
        "vmul.t C700, C210, C100;" // -a * p.x
        "vmul.t C710, C200, C110;" // b * p.y
        "vsub.t C220, C700, C710;");

    // Negate -a
    asm("vneg.t C210, C210;");
}

Vec4i TriangleBBOX()
{
    asm(
        // Lower bound
        "vmin.p C700, R100, R101;"
        "vmin.p C700, C700, R102;"

        // Upper bound
        "vmax.p C702, R100, R101;"
        "vmax.p C702, C702, R102;");

    // Fit to screen bounds
    asm(
        "vmax.p C700, C700, C020;" // Lower corner. C020 is the 0 vector
        "vmin.p C702, C702, R003;" // Upper corner
    );

    // TODO: There might be a way to perform all of these in the VFPU
    // Translate to "grid-space"
    LOAD_VEC4_COL(7, 1, GRID_SIZE_VEC4);
    asm("vdiv.q C700, C700, C710;"); // BBOX / GRID_SIZE

    Vec4f VFPU_ALIGN bboxDivGridSize;
    STORE_VEC4_COL(7, 0, &bboxDivGridSize);

    Vec4i bbox{
        GRID_SIZE * static_cast<int_psp>(bboxDivGridSize.x),
        GRID_SIZE * static_cast<int_psp>(bboxDivGridSize.y),
        GRID_SIZE * static_cast<int_psp>(bboxDivGridSize.z),
        GRID_SIZE * static_cast<int_psp>(bboxDivGridSize.w)};

    return bbox;
}

void CalculateBarycentricCoordinates()
{
    // Clear target matrixes
    asm(
        "vmzero.q M300;"
        "vmzero.q M400;"
        "vmzero.q M500;");

    // E_SUM(M700) <- E0(M000) + E1(M100) + E2(M200)
    asm(
        "vadd.q R700, R000, R100;"
        "vadd.q R700, R700, R200;"
        "vadd.q R701, R001, R101;"
        "vadd.q R701, R701, R201;"
        "vadd.q R702, R002, R102;"
        "vadd.q R702, R702, R202;"
        "vadd.q R703, R003, R103;"
        "vadd.q R703, R703, R203;");

    // U(M300) <- E1(M100) / E_SUM(M700)
    asm(
        "vdiv.q R300, R100, R700;"
        "vdiv.q R400, R101, R701;"
        "vdiv.q R500, R102, R702;"
        "vdiv.q R600, R103, R703;");

    // V(M400) <- E1(M200) / E_SUM(M700)
    asm(
        "vdiv.q R301, R200, R700;"
        "vdiv.q R401, R201, R701;"
        "vdiv.q R501, R202, R702;"
        "vdiv.q R601, R203, R703;");

    // Set a 1-filled vector to later perform w = 1-u-w
    asm(
        "vone.q R700;");

    // W(M500) <- 1(R700) - U(M300) - V(M400)
    asm(
        "vsub.q R302, R700, R300;"
        "vsub.q R302, R302, R301;"
        "vsub.q R402, R700, R400;"
        "vsub.q R402, R402, R401;"
        "vsub.q R502, R700, R500;"
        "vsub.q R502, R502, R501;"
        "vsub.q R602, R700, R600;"
        "vsub.q R602, R602, R601;");
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
    Vec3f baryCoords{BarycentricCoordinates(pixel, edgeFuncs)};

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

void InitializeVFPUForRasterization()
{
    // Set 0 and 1 vectors first
    asm("vzero.q C020;"
        "vone.q C030;");

    // Load half width/height and fill the other rows by addition
    LOAD_VEC2_ROW_L(0, 0, HALF_SCREEN_VEC2);
    asm(
        "vadd.p R001, R000, C020;"
        "vadd.p R002, R000, C020;");

    // Load max width/height
    LOAD_VEC2_ROW_L(0, 3, MAX_SCREEN_VEC2);
}

std::vector<Fragment> Rasterize(const Mesh &mesh, const BufferVertexData *buffer)
{
    std::vector<Fragment> fragments;
    for (size_t i{0}; i < mesh.triangleCount; ++i)
    {
        const Vec3i &tri{mesh.triangles[i]};
        bool visible{TriangleIsVisible(tri, buffer)};

        if (visible)
        {
            RasterizeTriangle(tri, buffer, fragments);
        }
    }

    return fragments;
}