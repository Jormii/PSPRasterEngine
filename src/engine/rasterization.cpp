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
void EvaluateEdgeFunction(const Vec2f pixel);
int_psp CheckInsideTriangle();
void CalculateBarycentricCoordinates();

void CreateFragment(const Vec3i &tri, const BufferVertexData *buffer, std::vector<Fragment> &fragments, const EdgeFunction *edgeFuncs, int_psp x, int_psp y, const Vec2f &pixel);

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

            EvaluateEdgeFunction(pixel);
            int_psp inside{CheckInsideTriangle()};

            if (inside > 0)
            {
                CalculateBarycentricCoordinates();
            }
        }
    }
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

    // a+b
    asm("vadd.t C230, C210, C200");
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

void EvaluateEdgeFunction(const Vec2f pixel)
{
    // Load pixel into VFPU
    LOAD_VEC2_ROW_L(7, 0, &pixel);
    asm(
        // Other rows are result of adding a 0 vector
        "vadd.p R701, R700, C020;"
        "vadd.p R702, R700, C020;");

    // Evaluate for arg pixel
    asm(
        "vmul.t C720, C210, C700;" // A*X
        "vmul.t C730, C200, C710;" // B*Y
        "vadd.t C300, C720, C730;" // A*X + B*Y
        "vadd.t C300, C300, C220;" // (A*X + B*Y) + c
    );

    // Add a, b and a+b to evaluate the other pixels in the grid
    asm(
        "vadd.t C310, C300, C210;" // (x+1, y)
        "vadd.t C320, C300, C200;" // (x, y+1)
        "vadd.t C330, C300, C230;" // (x+1, y+1)
    );
}

int_psp CheckInsideTriangle()
{
// zero(x) = 1 - abs(sign(x))
#define EQ_ZERO(SRC, DST)                    \
    {                                        \
        asm(                                 \
            "vsgn.t C700, " #SRC ";"         \
            "vabs.t C700, C700;"             \
            "vsub.t " #DST ", C030, C700;"); \
    }

// gt_zero(x) = (1 - zero(x))*u(x)
// u(x) = (x + abs(x)) / 2x
// "vmax.t" requiered to avoid -inf problems
#define GT_ZERO(SRC, ZERO_DST, GT_ZERO_DST)                      \
    {                                                            \
        EQ_ZERO(SRC, ZERO_DST);                                  \
        asm(                                                     \
            "vsub.t " #GT_ZERO_DST ", C030, " #ZERO_DST ";"      \
                                                                 \
            "vabs.t C700, " #SRC ";"                             \
            "vadd.t C700, " #SRC ", C700;"                       \
            "vadd.t C710, " #SRC ", " #SRC ";"                   \
            "vdiv.t C720, C700, C710;"                           \
            "vmax.t C720, C720, C020;"                           \
                                                                 \
            "vmul.t " #GT_ZERO_DST ", " #GT_ZERO_DST ", C720;"); \
    }

// gte_zero(x) = max{zero(x), ge(x)}
#define GTE_ZERO(SRC, ZERO_DST, GTE_ZERO_DST)                               \
    {                                                                       \
        GT_ZERO(SRC, ZERO_DST, GTE_ZERO_DST);                               \
        asm("vmax.t " #GTE_ZERO_DST ", " #ZERO_DST ", " #GTE_ZERO_DST ";"); \
    }

// Inside = (EI > 0) OR (EI = 0 AND A > 0) OR (EI = 0 AND A = 0 B >= 0)
#define INSIDE(DST, EI_EQ_ZERO, EI_GT_ZERO, A_EQ_ZERO, A_GT_ZERO, B_GTE_ZERO) \
    {                                                                         \
        asm(                                                                  \
            "vmul.t C700, " #EI_EQ_ZERO ", " #A_GT_ZERO ";"                   \
                                                                              \
            "vmul.t C710, " #EI_EQ_ZERO ", " #A_EQ_ZERO ";"                   \
            "vmul.t C710, C710, " #B_GTE_ZERO ";"                             \
                                                                              \
            "vadd.t C720, " #EI_GT_ZERO ", C700;"                             \
            "vadd.t C720, C720, C710;"                                        \
                                                                              \
            "vmul.s " #DST ", S720, S721;"                                    \
            "vmul.s " #DST ", " #DST ", S722;");                              \
    }

    // Initialize variables in the first place for a and b. Stored in M600
    GT_ZERO(C210, C500, C510);
    GTE_ZERO(C200, C520, C530);

    // Check first pixel
    asm("vzero.p C700;");
    GT_ZERO(C300, C600, C610);
    asm("vzero.p C700;");
    INSIDE(S303, C600, C610, C500, C510, C530);

    // Second pixel
    GT_ZERO(C310, C600, C610);
    INSIDE(S313, C600, C610, C500, C510, C530);

    // Third pixel
    GT_ZERO(C320, C600, C610);
    INSIDE(S323, C600, C610, C500, C510, C530);

    // Fourth pixel
    GT_ZERO(C330, C600, C610);
    INSIDE(S333, C600, C610, C500, C510, C530);

    // Gather how many pixels are inside
    int_psp count;
    asm(
        "vadd.s S700, S303, S313;"
        "vadd.s S700, S700, S323;"
        "vadd.s S700, S700, S333;"

        "vf2in.s S700, S700, 0;"
        "sv.s S700, 0(%0)"
        :
        : "r"(&count)
        :);

    return count;

#undef EQ_ZERO
#undef GT_ZERO
#undef GTE_ZERO
}

void CalculateBarycentricCoordinates()
{
    // Clear target matrix
    asm("vmzero.q M400;");

    // E0 + E1 + E2
    asm(
        "vadd.q R700, R300, R301;"
        "vadd.q R700, R700, R302;");

    // Calculate weights
    asm(
        "vdiv.q R400, R301, R700;" // U <- E1 / Sum{E}
        "vdiv.q R401, R302, R700;" // V <- E2 / Sum{E}

        // W <- 1-U-V
        "vsub.q R402, C030, R400;"
        "vsub.q R402, R402, R401;");
}

void CreateFragment(const Vec3i &tri, const BufferVertexData *buffer, std::vector<Fragment> &fragments, const EdgeFunction *edgeFuncs, int_psp x, int_psp y, const Vec2f &pixel)
{
    Vec3f baryCoords;

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