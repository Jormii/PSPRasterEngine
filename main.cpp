#include <cmath>

#include "./include/engine/engine.hpp"
#include "./include/engine/utils.hpp"

#include "./Sample Meshes/tetra.hpp"

#define REDIRECT
#ifdef REDIRECT
constexpr size_t WIDTH{140};
constexpr size_t HEIGHT{35};
#else
constexpr size_t WIDTH{48};
constexpr size_t HEIGHT{14};
#endif

void CustomVS(const DrawMatrices &matrices, const VertexData &vertexData, BufferVertexData *out)
{
    Vec4f vHomo{vertexData.position, 1.0f};
    Vec4f vertexTransformed{matrices.mvp * vHomo};

    out->positionHomo = vertexTransformed;
    out->color = vertexData.color;
}

void CustomFS(const Fragment &fragment, FSOut &out)
{
    out.depth = fragment.depth;
    out.color = fragment.color;
}

int main()
{
    InitializeContext(WIDTH, HEIGHT);

    // Set up matrices
    float_psp angle{0.25 * M_PI};
    float_psp distance{3.5f};
    float_psp cameraX{distance * cosf(angle)};
    float_psp cameraZ{distance * sinf(angle)};

    Mat4f model{Mat4f::IDENTITY};
    Mat4f view{LookAt(
        Vec3f{cameraX, 0.0f, cameraZ},
        Vec3f{0.0f, 0.0f, 0.0f},
        Vec3f{0.0f, 1.0f, 0.0f})};
    Mat4f projection{PerspectiveProjFov(WIDTH, HEIGHT, 60.0f * (M_PI / 180), 1.0f, 10.0f)};

    DrawMatrices matrices{model, view, projection};

    // Draw call
    ClearColorBuffer(RGBA{0, 0, 0, 255}); // Black
    ClearDepthBuffer(9999.0f);
    Draw(tetraMesh, matrices, &CustomVS, &CustomFS);

    // Render
    RenderToConsole();

    return 0;
}