#include "rasterization.hpp"

#include <cmath>

#include "debug.hpp"
#include "constants.hpp"
#include "edge.hpp"

#include "vec2i.hpp"
#include "vec4i.hpp"

/***
 * NOTES ABOUT RASTERIZATION
 * General procedure: If a triangle is visible, then rasterize it
 * Rasterization steps:
 *  - BBOX: In terms of grid, aka, values returned are multiples of GRID_SIZE=4
 *  - Edge function:
 *      Results are stored in matrixes M000, M100 & M200
 *      M300, M400, M500 store ei(x, y) for the bottom left pixel in the grid and parameters a, b of e.
 *          Their 4th row are used for temporal calculations
 *      R600 stores a constant (0, 1, 2, 3) vector used to calculate ei(X, Y) being XY the pixels in the grid
 *  - Weights: TODO
 *  - Interpolation: TODO
 *  - Visibility and fragment creation: TODO
 */

#define GRID_SIZE 4
#define GRID_CELLS (GRID_SIZE * GRID_SIZE)

const Vec4f __attribute__((aligned(16))) ZERO_TO_THREE{0.0f, 1.0f, 2.0f, 3.0f};

constexpr float_psp WIDTH_MINUS_1{PSP_WIDTH - 1.0f};
constexpr float_psp HEIGHT_MINUS_1{PSP_HEIGHT - 1.0f};

bool TriangleIsVisible(const Vec3i &tri, const BufferVertexData *buffer);
void RasterizeTriangle(const Vec3i &tri, const BufferVertexData *buffer, const Vec2f *screenSpace, std::vector<Fragment> &fragments, size_t width, size_t height);
Vec4i TriangleBBOX(const Vec2f &a, const Vec2f &b, const Vec2f &c);
void CalculateEdgeFunctions(const Vec2f &pixel, const EdgeFunction *edges);
void CalculateBarycentricCoordinates();

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

            DebugStart(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_WITHIN_TRIANGLE);
            DebugEnd(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_WITHIN_TRIANGLE);

            // TODO: Not correct. Temporal.
            DebugStart(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_INTERPOLATION);
            CreateFragment(tri, buffer, fragments, edges, x, y, pixel);
            DebugEnd(DebugIDs::RASTERIZATION_RASTERIZE_TRIANGLE_INTERPOLATION);
        }
        pixel.y += static_cast<float_psp>(GRID_SIZE);
    }

    pixel.x += static_cast<float_psp>(GRID_SIZE);
    pixel.y = startY;
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
    Vec4i __attribute__((aligned(16))) bbox;
    asm(
        "vf2in.q R003, R003, 0;"
        "sv.q R003, 0(%0);"
        :
        : "r"(&bbox)
        :);

    // Scale to fit grid
    bbox.x = (bbox.x) - (bbox.x % GRID_SIZE);
    bbox.y = (bbox.y) - (bbox.y % GRID_SIZE);
    bbox.z = (bbox.z) - (bbox.z % GRID_SIZE);
    bbox.w = (bbox.w) - (bbox.w % GRID_SIZE);

    return bbox;
}

void CalculateEdgeFunctions(const Vec2f &pixel, const EdgeFunction *edges)
{
    // Set up
    asm(
        "lv.q R600, 0(%0);"
        :
        : "r"(&ZERO_TO_THREE)
        :);

    // Store edges' variables
    float_psp f0{edges[0].Evaluate(pixel)};
    asm(
        "lv.s S300, 0(%0);  lv.s S310, 0(%0); lv.s S320, 0(%0);  lv.s S330, 0(%0);"
        "lv.s S301, 0(%1);  lv.s S311, 0(%1); lv.s S321, 0(%1);  lv.s S331, 0(%1);"
        "lv.s S302, 0(%2);  lv.s S312, 0(%2); lv.s S322, 0(%2);  lv.s S332, 0(%2);"
        :
        : "r"(&f0), "r"(&edges[0].a), "r"(&edges[0].b)
        :);

    float_psp f1{edges[1].Evaluate(pixel)};
    asm(
        "lv.s S400, 0(%0);  lv.s S410, 0(%0); lv.s S420, 0(%0);  lv.s S430, 0(%0);"
        "lv.s S401, 0(%1);  lv.s S411, 0(%1); lv.s S421, 0(%1);  lv.s S431, 0(%1);"
        "lv.s S402, 0(%2);  lv.s S412, 0(%2); lv.s S422, 0(%2);  lv.s S432, 0(%2);"
        :
        : "r"(&f1), "r"(&edges[1].a), "r"(&edges[1].b)
        :);

    float_psp f2{edges[2].Evaluate(pixel)};
    asm(
        "lv.s S500, 0(%0);  lv.s S510, 0(%0); lv.s S520, 0(%0);  lv.s S530, 0(%0);"
        "lv.s S501, 0(%1);  lv.s S511, 0(%1); lv.s S521, 0(%1);  lv.s S531, 0(%1);"
        "lv.s S502, 0(%2);  lv.s S512, 0(%2); lv.s S522, 0(%2);  lv.s S532, 0(%2);"
        :
        : "r"(&f2), "r"(&edges[2].a), "r"(&edges[2].b)
        :);

    // Calculate
    asm(
        // Edge 0
        // M000(rows) <- I*A0
        "vmul.q R000, R301, R600;"
        "vmul.q R001, R301, R600;"
        "vmul.q R002, R301, R600;"
        "vmul.q R003, R301, R600;"

        // R303 <- J*B0
        "vmul.q R303, R302, R600;"

        // M000(columns) <- M000(columns) + J*B0
        "vadd.q C000, C000, R303;"
        "vadd.q C010, C010, R303;"
        "vadd.q C020, C020, R303;"
        "vadd.q C030, C030, R303;"

        // M000 <- M000 + R300 (e0(x, y))
        "vadd.q C000, C000, R300;"
        "vadd.q C010, C010, R300;"
        "vadd.q C020, C020, R300;"
        "vadd.q C030, C030, R300;");

    asm(
        // Edge 1
        // M100(rows) <- I*A1
        "vmul.q R100, R401, R600;"
        "vmul.q R101, R401, R600;"
        "vmul.q R102, R401, R600;"
        "vmul.q R103, R401, R600;"

        // R403 <- J*B1
        "vmul.q R403, R402, R600;"

        // M100(columns) <- M100(columns) + J*B1
        "vadd.q C100, C100, R403;"
        "vadd.q C110, C110, R403;"
        "vadd.q C120, C120, R403;"
        "vadd.q C130, C130, R403;"

        // M100 <- M100 + R400 (e1(x, y))
        "vadd.q C100, C100, R400;"
        "vadd.q C110, C110, R400;"
        "vadd.q C120, C120, R400;"
        "vadd.q C130, C130, R400;");

    asm(
        // Edge 2
        // M200(rows) <- I*A2
        "vmul.q R200, R501, R600;"
        "vmul.q R201, R501, R600;"
        "vmul.q R202, R501, R600;"
        "vmul.q R203, R501, R600;"

        // R503 <- J*B2
        "vmul.q R503, R402, R600;"

        // M200(columns) <- M200(columns) + J*B2
        "vadd.q C200, C200, R503;"
        "vadd.q C210, C210, R503;"
        "vadd.q C220, C220, R503;"
        "vadd.q C230, C230, R503;"

        // M200 <- M200 + R500 (e1(x, y))
        "vadd.q C200, C200, R500;"
        "vadd.q C210, C210, R500;"
        "vadd.q C220, C220, R500;"
        "vadd.q C230, C230, R500;");
}

void CalculateBarycentricCoordinates()
{
    // E_SUM(M700) <- E0(M000) + E1(M100) + E20(M200)
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
        "vdiv.q R301, R101, R701;"
        "vdiv.q R302, R102, R702;"
        "vdiv.q R303, R103, R703;");

    // V(M400) <- E1(M200) / E_SUM(M700)
    asm(
        "vdiv.q R400, R200, R700;"
        "vdiv.q R401, R201, R701;"
        "vdiv.q R402, R202, R702;"
        "vdiv.q R403, R203, R703;");

    // W(M500) <- 1(R700) - U(M300) - V(M400)
    asm(
        "vone.q R700;"

        "vsub.q R500, R700, R300;"
        "vsub.q R500, R500, R400;"
        "vsub.q R501, R700, R301;"
        "vsub.q R501, R501, R401;"
        "vsub.q R502, R700, R302;"
        "vsub.q R502, R502, R402;"
        "vsub.q R503, R700, R303;"
        "vsub.q R503, R503, R403;");
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