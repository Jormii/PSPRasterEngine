#include "rasterization.hpp"

#include <cmath>

#include "vfpu.h"
#include "vfpu_funcs.h"

#include "debug.hpp"
#include "constants.hpp"
#include "edge.hpp"

#include "vec2i.hpp"
#include "vec4i.hpp"

/**
 * Constants
 */
#define ZERO_VECTOR_V4 "C020"
#define ONE_VECTOR_V4 "C030"
#define HALF_SCREEN_0_V2 "R000"
#define HALF_SCREEN_1_V2 "R001"
#define HALF_SCREEN_2_V2 "R002"
#define HALF_SCREEN_X_V3 "C000"
#define HALF_SCREEN_Y_V3 "C010"
#define MAX_SCREEN_BOUNDS_V2 "R003"

/**
 * Screen coordinates
 */
#define SCREEN_COORDS_0_V2 "R100"
#define SCREEN_COORDS_1_V2 "R101"
#define SCREEN_COORDS_2_V2 "R102"
#define SCREEN_COORDS_X_V3 "C100"
#define SCREEN_COORDS_Y_V3 "C110"

/**
 * Edge functions
 */
#define E0_V4 "R200"
#define E1_V4 "R201"
#define E2_V4 "R202"
#define EI_A_V3 "C210"
#define EI_B_V3 "C200"
#define EI_C_V3 "C220"
#define EI_A_PLUS_B_V3 "C230"

/**
 * Evaluated edge
 */
#define F_EI_X_Y_V3 "C300"
#define F_EI_X_PLUS_1_Y_V3 "C310"
#define F_EI_X_Y_PLUS_1_V3 "C320"
#define F_EI_X_PLUS_1_Y_PLUS_1_V3 "C330"
#define F_E0_V4 "R300"
#define F_E1_V4 "R301"
#define F_E2_V4 "R302"

/**
 * Inside triangle
 */
#define INSIDE_TRIANGLE_V4 "R303"
#define X_Y_INSIDE_V1 "S303"
#define X_PLUS_1_Y_INSIDE_V1 "S313"
#define X_Y_PLUS_1_INSIDE_V1 "S323"
#define X_PLUS_1_Y_PLUS_1_INSIDE_V1 "S333"

/**
 * Barycentric coordinates
 */
#define BARYCENTRIC_MATRIX "M400"
#define BARYCENTRIC_U_V4 "R400"
#define BARYCENTRIC_V_V4 "R401"
#define BARYCENTRIC_W_V4 "R402"

/**
 * Interpolation
 */
#define INTERPOLATION_MATRIX "M500"

bool TriangleIsVisible(const Vec3i &tri, const BufferVertexData *buffer);
void RasterizeTriangle(const Vec3i &tri, const BufferVertexData *buffer, std::vector<Fragment> &fragments);
void UploadScreenCoordinatesToVFPU(const Vec3i &tri, const BufferVertexData *buffer);
void InitializeEdgeFunctions();
Vec4i TriangleBBOX();
void EvaluateEdgeFunction(const Vec2f pixel);
int_psp CheckInsideTriangle();
void CalculateBarycentricCoordinates();
void Interpolate(int_psp x, int_psp y, const BufferVertexData &a, const BufferVertexData &b, const BufferVertexData &c, std::vector<Fragment> &fragments);

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

    DebugStart(DebugIDs::TRIANGLE_TRAVERSAL);
    for (int_psp y{bbox.y}; y <= bbox.w; y += GRID_SIZE)
    {
        for (int_psp x{bbox.x}; x <= bbox.z; x += GRID_SIZE)
        {
            Vec2f pixel{
                static_cast<float_psp>(x) + 0.5f,
                static_cast<float_psp>(y) + 0.5f};

            DebugStart(DebugIDs::EVALUATE_EDGE_FUNCTION);
            EvaluateEdgeFunction(pixel);
            DebugEnd(DebugIDs::EVALUATE_EDGE_FUNCTION);

            DebugStart(DebugIDs::INSIDE_TRIANGLE);
            int_psp inside{CheckInsideTriangle()};
            DebugEnd(DebugIDs::INSIDE_TRIANGLE);

            if (inside > 0)
            {
                DebugStart(DebugIDs::BARY_COORDS);
                CalculateBarycentricCoordinates();
                DebugEnd(DebugIDs::BARY_COORDS);

                DebugStart(DebugIDs::INTERPOLATE);
                Interpolate(x, y, buffer[tri.x], buffer[tri.y], buffer[tri.z], fragments);
                DebugEnd(DebugIDs::INTERPOLATE);
            }
        }
    }
    DebugEnd(DebugIDs::TRIANGLE_TRAVERSAL);
}

void UploadScreenCoordinatesToVFPU(const Vec3i &tri, const BufferVertexData *buffer)
{
    VFPU_FUN_LOAD_V2_ROW_C01(7, 0, &(buffer[tri.x].position));
    VFPU_FUN_LOAD_V2_ROW_C01(7, 1, &(buffer[tri.y].position));
    VFPU_FUN_LOAD_V2_ROW_C01(7, 2, &(buffer[tri.z].position));

    // Add 1
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V3, SCREEN_COORDS_X_V3, "C700", ONE_VECTOR_V4);
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V3, SCREEN_COORDS_Y_V3, "C710", ONE_VECTOR_V4);

    // Multiply by half width/height
    VFPU_INST_BINARY(VFPU_OP_V_MULT, VFPU_V3, SCREEN_COORDS_X_V3, SCREEN_COORDS_X_V3, HALF_SCREEN_X_V3);
    VFPU_INST_BINARY(VFPU_OP_V_MULT, VFPU_V3, SCREEN_COORDS_Y_V3, SCREEN_COORDS_Y_V3, HALF_SCREEN_Y_V3);
}

void InitializeEdgeFunctions()
{
    // Calculate vectors => Parameters b, -a
    // b in C200, -a in C210
    VFPU_INST_BINARY(VFPU_OP_V_SUB, VFPU_V2, E0_V4, SCREEN_COORDS_1_V2, SCREEN_COORDS_0_V2);
    VFPU_INST_BINARY(VFPU_OP_V_SUB, VFPU_V2, E1_V4, SCREEN_COORDS_2_V2, SCREEN_COORDS_1_V2);
    VFPU_INST_BINARY(VFPU_OP_V_SUB, VFPU_V2, E2_V4, SCREEN_COORDS_0_V2, SCREEN_COORDS_2_V2);

    // Calculate c
    VFPU_INST_BINARY(VFPU_OP_V_MULT, VFPU_V3, "C700", EI_A_V3, SCREEN_COORDS_X_V3); // -a * p.x
    VFPU_INST_BINARY(VFPU_OP_V_MULT, VFPU_V3, "C710", EI_B_V3, SCREEN_COORDS_Y_V3); // b * p.y
    VFPU_INST_BINARY(VFPU_OP_V_SUB, VFPU_V3, EI_C_V3, "C700", "C710");

    // Negate -a
    VFPU_INST_UNARY(VFPU_OP_V_NEG, VFPU_V3, EI_A_V3, EI_A_V3);

    // a+b
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V3, EI_A_PLUS_B_V3, EI_A_V3, EI_B_V3);
}

Vec4i TriangleBBOX()
{
    // Lower bound
    VFPU_INST_BINARY(VFPU_OP_V_MIN, VFPU_V2, "C700", SCREEN_COORDS_0_V2, SCREEN_COORDS_1_V2);
    VFPU_INST_BINARY(VFPU_OP_V_MIN, VFPU_V2, "C700", "C700", SCREEN_COORDS_2_V2);

    // Upper bound
    VFPU_INST_BINARY(VFPU_OP_V_MAX, VFPU_V2, "C702", SCREEN_COORDS_0_V2, SCREEN_COORDS_1_V2);
    VFPU_INST_BINARY(VFPU_OP_V_MAX, VFPU_V2, "C702", "C702", SCREEN_COORDS_2_V2);

    // Fit to screen bounds
    VFPU_INST_BINARY(VFPU_OP_V_MAX, VFPU_V2, "C700", "C700", ZERO_VECTOR_V4);
    VFPU_INST_BINARY(VFPU_OP_V_MIN, VFPU_V2, "C702", "C702", MAX_SCREEN_BOUNDS_V2);

    // TODO: There might be a way to perform all of these in the VFPU
    // Translate to "grid-space"
    VFPU_INST_MEMORY(VFPU_OP_LOAD, VFPU_V4, "C710", GRID_SIZE_VEC4, 0);
    VFPU_INST_BINARY(VFPU_OP_V_DIV, VFPU_V4, "C700", "C700", "C710"); // BBOX / GRID_SIZE

    Vec4f VFPU_ALIGN bboxDivGridSize;
    VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V4, "C700", &bboxDivGridSize, 0);

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
    VFPU_FUN_LOAD_V2_ROW_C01(7, 0, &pixel);
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V2, "R701", "R700", ZERO_VECTOR_V4); // Other rows are "loaded" adding a 0 vector
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V2, "R702", "R700", ZERO_VECTOR_V4);

    // Evaluate for arg pixel
    VFPU_INST_BINARY(VFPU_OP_V_MULT, VFPU_V3, "C720", EI_A_V3, "C700");          // A*X
    VFPU_INST_BINARY(VFPU_OP_V_MULT, VFPU_V3, "C730", EI_B_V3, "C710");          // B*Y
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V3, F_EI_X_Y_V3, "C720", "C730");       // A*X + B*Y
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V3, F_EI_X_Y_V3, F_EI_X_Y_V3, EI_C_V3); // (A*X + B*Y) + C

    // Add a, b and a+b to evaluate the other pixels in the grid
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V3, F_EI_X_PLUS_1_Y_V3, F_EI_X_Y_V3, EI_A_V3);               // (x+1, y)
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V3, F_EI_X_Y_PLUS_1_V3, F_EI_X_Y_V3, EI_B_V3);               // (x, y+1)
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V3, F_EI_X_PLUS_1_Y_PLUS_1_V3, F_EI_X_Y_V3, EI_A_PLUS_B_V3); // (x+1, y+1)
}

int_psp CheckInsideTriangle()
{
// Inside = (EI > 0) OR (EI = 0 AND A > 0) OR (EI = 0 AND A = 0 B >= 0)
#define INSIDE(DST, EI_EQ_ZERO, EI_GT_ZERO, A_EQ_ZERO, A_GT_ZERO, B_GTE_ZERO)     \
    {                                                                             \
        VFPU_INST_BINARY(VFPU_OP_V_MULT, VFPU_V3, "C700", EI_EQ_ZERO, A_GT_ZERO); \
                                                                                  \
        VFPU_INST_BINARY(VFPU_OP_V_MULT, VFPU_V3, "C710", EI_EQ_ZERO, A_EQ_ZERO); \
        VFPU_INST_BINARY(VFPU_OP_V_MULT, VFPU_V3, "C710", "C710", B_GTE_ZERO);    \
                                                                                  \
        VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V3, "C720", EI_GT_ZERO, "C700");     \
        VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V3, "C720", "C720", "C710");         \
                                                                                  \
        VFPU_INST_BINARY(VFPU_OP_V_MULT, VFPU_V1, DST, "S720", "S721");           \
        VFPU_INST_BINARY(VFPU_OP_V_MULT, VFPU_V1, DST, DST, "S722");              \
    }

#define A_EQ_ZERO "C500"
#define A_GT_ZERO "C510"
#define B_EQ_ZERO "C520"
#define B_GTE_ZERO "C530"
#define EI_EQ_ZERO "C600"
#define EI_GT_ZERO "C610"

    // Initialize variables in the first place for a and b. Stored in M500
    VFPU_FUN_GT_ZERO(VFPU_V3, EI_A_V3, A_EQ_ZERO, A_GT_ZERO);
    VFPU_FUN_GTE_ZERO(VFPU_V3, EI_B_V3, B_EQ_ZERO, B_GTE_ZERO);

    // TODO: Can likely be improved
    // Check first pixel
    VFPU_FUN_GT_ZERO(VFPU_V3, F_EI_X_Y_V3, EI_EQ_ZERO, EI_GT_ZERO);
    INSIDE(X_Y_INSIDE_V1, EI_EQ_ZERO, EI_GT_ZERO, A_EQ_ZERO, A_GT_ZERO, B_GTE_ZERO);

    // Second pixel
    VFPU_FUN_GT_ZERO(VFPU_V3, F_EI_X_PLUS_1_Y_V3, EI_EQ_ZERO, EI_GT_ZERO);
    INSIDE(X_PLUS_1_Y_INSIDE_V1, EI_EQ_ZERO, EI_GT_ZERO, A_EQ_ZERO, A_GT_ZERO, B_GTE_ZERO);

    // Third pixel
    VFPU_FUN_GT_ZERO(VFPU_V3, F_EI_X_Y_PLUS_1_V3, EI_EQ_ZERO, EI_GT_ZERO);
    INSIDE(X_Y_PLUS_1_INSIDE_V1, EI_EQ_ZERO, EI_GT_ZERO, A_EQ_ZERO, A_GT_ZERO, B_GTE_ZERO);

    // Last pixel
    VFPU_FUN_GT_ZERO(VFPU_V3, F_EI_X_PLUS_1_Y_PLUS_1_V3, EI_EQ_ZERO, EI_GT_ZERO);
    INSIDE(X_PLUS_1_Y_PLUS_1_INSIDE_V1, EI_EQ_ZERO, EI_GT_ZERO, A_EQ_ZERO, A_GT_ZERO, B_GTE_ZERO);

    // Gather how many pixels are inside
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V1, "S700", X_Y_INSIDE_V1, X_PLUS_1_Y_INSIDE_V1);
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V1, "S700", "S700", X_Y_PLUS_1_INSIDE_V1);
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V1, "S700", "S700", X_PLUS_1_Y_PLUS_1_INSIDE_V1);

    float_psp count;
    VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V1, "S700", &count, 0);

    return static_cast<float_psp>(count);
}

void CalculateBarycentricCoordinates()
{
    // Clear target matrix
    VFPU_INST_NO_OPERANDS(VFPU_OP_M_ZERO, VFPU_V4, BARYCENTRIC_MATRIX);

    // f(E0) + f(E1) + f(E2)
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V4, "R700", F_E0_V4, F_E1_V4);
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V4, "R700", "R700", F_E2_V4);

    // Calculate weights
    VFPU_INST_BINARY(VFPU_OP_V_DIV, VFPU_V4, BARYCENTRIC_U_V4, F_E1_V4, "R700");                 // U <- f(E1) / Sum{f(EI)}
    VFPU_INST_BINARY(VFPU_OP_V_DIV, VFPU_V4, BARYCENTRIC_V_V4, F_E2_V4, "R700");                 // V <- f(E2) / Sum{f(EI)}
    VFPU_INST_BINARY(VFPU_OP_V_SUB, VFPU_V4, BARYCENTRIC_W_V4, ONE_VECTOR_V4, BARYCENTRIC_U_V4); // W <- 1-U-V
    VFPU_INST_BINARY(VFPU_OP_V_SUB, VFPU_V4, BARYCENTRIC_W_V4, BARYCENTRIC_W_V4, BARYCENTRIC_V_V4);
}

void Interpolate(int_psp x, int_psp y, const BufferVertexData &a, const BufferVertexData &b, const BufferVertexData &c, std::vector<Fragment> &fragments)
{
#define INTERPOLATE()                                                                                \
    {                                                                                                \
        VFPU_INST_BINARY(VFPU_OP_M_MULT, VFPU_V4, "M600", INTERPOLATION_MATRIX, BARYCENTRIC_MATRIX); \
    }

#define STORE_V1(PTR_1, PTR_2, PTR_3, PTR_4)                        \
    {                                                               \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V1, "S600", PTR_1, 0); \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V1, "S610", PTR_2, 0); \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V1, "S620", PTR_3, 0); \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V1, "S630", PTR_4, 0); \
    }

#define STORE_V4(PTR_1, PTR_2, PTR_3, PTR_4)                        \
    {                                                               \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V4, "C600", PTR_1, 0); \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V4, "C610", PTR_2, 0); \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V4, "C620", PTR_3, 0); \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V4, "C630", PTR_4, 0); \
    }

#define STORE_OTHER_V(STORE_COL_FUN, PTR_1, PTR_2, PTR_3, PTR_4) \
    {                                                            \
        STORE_COL_FUN(6, 0, PTR_1);                              \
        STORE_COL_FUN(6, 1, PTR_2);                              \
        STORE_COL_FUN(6, 2, PTR_3);                              \
        STORE_COL_FUN(6, 3, PTR_4);                              \
    }

    Fragment newFragments[4];
    newFragments[0].xScreenCoord = x;
    newFragments[0].yScreenCoord = y;
    newFragments[1].xScreenCoord = x + 1;
    newFragments[1].yScreenCoord = y;
    newFragments[2].xScreenCoord = x;
    newFragments[2].yScreenCoord = y + 1;
    newFragments[3].xScreenCoord = x + 1;
    newFragments[3].yScreenCoord = y + 1;

    // Clear target matrix
    VFPU_INST_NO_OPERANDS(VFPU_OP_M_ZERO, VFPU_V4, INTERPOLATION_MATRIX);

    // Depth
    VFPU_INST_MEMORY(VFPU_OP_LOAD, VFPU_V1, "S500", &(a.position.z), 0);
    VFPU_INST_MEMORY(VFPU_OP_LOAD, VFPU_V1, "S510", &(b.position.z), 0);
    VFPU_INST_MEMORY(VFPU_OP_LOAD, VFPU_V1, "S520", &(c.position.z), 0);

    INTERPOLATE();
    STORE_V1(&(newFragments[0].depth), &(newFragments[1].depth), &(newFragments[2].depth), &(newFragments[3].depth));

    // Viewpos
    VFPU_FUN_LOAD_V3_COL(5, 0, &(a.viewPos));
    VFPU_FUN_LOAD_V3_COL(5, 1, &(b.viewPos));
    VFPU_FUN_LOAD_V3_COL(5, 2, &(c.viewPos));

    INTERPOLATE();
    STORE_OTHER_V(VFPU_FUN_STORE_V3_COL, &(newFragments[0].viewPos), &(newFragments[1].viewPos), &(newFragments[2].viewPos), &(newFragments[3].viewPos));

    // Normal
    VFPU_FUN_LOAD_V3_COL(5, 0, &(a.normal));
    VFPU_FUN_LOAD_V3_COL(5, 1, &(b.normal));
    VFPU_FUN_LOAD_V3_COL(5, 2, &(c.normal));

    INTERPOLATE();
    STORE_OTHER_V(VFPU_FUN_STORE_V3_COL, &(newFragments[0].normal), &(newFragments[1].normal), &(newFragments[2].normal), &(newFragments[3].normal));

    // Color
    VFPU_INST_MEMORY(VFPU_OP_LOAD, VFPU_V4, "C500", &(a.color), 0);
    VFPU_INST_MEMORY(VFPU_OP_LOAD, VFPU_V4, "C510", &(b.color), 0);
    VFPU_INST_MEMORY(VFPU_OP_LOAD, VFPU_V4, "C520", &(c.color), 0);

    INTERPOLATE();
    STORE_V4(&(newFragments[0].color), &(newFragments[1].color), &(newFragments[2].color), &(newFragments[3].color));

    // UV
    VFPU_FUN_LOAD_V2_COL_R01(5, 0, &(a.uv));
    VFPU_FUN_LOAD_V2_COL_R01(5, 1, &(b.uv));
    VFPU_FUN_LOAD_V2_COL_R01(5, 2, &(c.uv));

    INTERPOLATE();
    STORE_OTHER_V(VFPU_FUN_STORE_V2_COL_R01, &(newFragments[0].uv), &(newFragments[1].uv), &(newFragments[2].uv), &(newFragments[3].uv))

    // Store those that are within the triangle
    float_psp VFPU_ALIGN within[4];
    VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V4, INSIDE_TRIANGLE_V4, &within, 0);
    for (int_psp i{0}; i < 4; ++i)
    {
        if (within[i])
        {
            fragments.push_back(newFragments[i]);
        }
    }
}

void InitializeVFPUForRasterization()
{
    DebugStart(DebugIDs::VFPU_RASTERIZATION_INIT);
    // Set 0 and 1 vectors first
    VFPU_INST_NO_OPERANDS(VFPU_OP_V_ZERO, VFPU_V4, ZERO_VECTOR_V4);
    VFPU_INST_NO_OPERANDS(VFPU_OP_V_ONE, VFPU_V4, ONE_VECTOR_V4);

    // Load half width/height and fill the other rows by addition
    VFPU_FUN_LOAD_V2_ROW_C01(0, 0, HALF_SCREEN_VEC2);
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V2, HALF_SCREEN_1_V2, HALF_SCREEN_0_V2, ZERO_VECTOR_V4);
    VFPU_INST_BINARY(VFPU_OP_V_ADD, VFPU_V2, HALF_SCREEN_2_V2, HALF_SCREEN_0_V2, ZERO_VECTOR_V4);

    // Load max width/height
    VFPU_FUN_LOAD_V2_ROW_C01(0, 3, MAX_SCREEN_VEC2);
    DebugEnd(DebugIDs::VFPU_RASTERIZATION_INIT);
}

std::vector<Fragment> Rasterize(const Mesh &mesh, const BufferVertexData *buffer)
{
    std::vector<Fragment> fragments;
    for (size_t i{0}; i < mesh.triangleCount; ++i)
    {
        const Vec3i &tri{mesh.triangles[i]};
        DebugStart(DebugIDs::TRIANGLE_VISIBILITY);
        bool visible{TriangleIsVisible(tri, buffer)};
        DebugEnd(DebugIDs::TRIANGLE_VISIBILITY);

        if (visible)
        {
            DebugStart(DebugIDs::RASTERIZE_TRIANGLE);
            RasterizeTriangle(tri, buffer, fragments);
            DebugEnd(DebugIDs::RASTERIZE_TRIANGLE);
        }
    }
    return fragments;
}