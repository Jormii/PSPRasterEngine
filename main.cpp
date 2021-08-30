#include <cmath>
#include <vector>

#include "./include/engine/engine.hpp"
#include "./include/engine/utils.hpp"

#include "./Sample Meshes/tetra.hpp"

#define REDIRECT
#ifdef REDIRECT
constexpr int_psp WIDTH{140};
constexpr int_psp HEIGHT{35};
#else
constexpr int_psp WIDTH{48};
constexpr int_psp HEIGHT{24};
#endif

VSOut CustomVertexShader(const VSIn &vsIn)
{
    Vec4f vHomo{vsIn.vertex, 1.0f};
    Vec4f vertexMVP{vsIn.modelViewProj * vHomo};

    return VSOut{vertexMVP, vHomo};
}

FSOut CustomFragmentShader(const FSIn &fsIn)
{
    RGBA color{255, 255, 255, 255};
    float_psp depth{fsIn.depth};

    return FSOut{color, depth};
}

int main()
{
    InitializeContext(WIDTH, HEIGHT);

    // Create render variables
    float_psp angle{0.25 * M_PI};
    float_psp distance{3.0f};
    float_psp cameraX{distance * cosf(angle)};
    float_psp cameraZ{distance * sinf(angle)};

    Mat4f model{Mat4f::IDENTITY};
    Mat4f view{LookAt(
        Vec3f{cameraX, 0.0f, cameraZ},
        Vec3f{0.0f, 0.0f, 0.0f},
        Vec3f{0.0f, 1.0f, 0.0f})};
    Mat4f projection{PerspectiveProjFov(WIDTH, HEIGHT, 60.0f * (M_PI / 180), 1.0f, 10.0f)};
    // Mat4f projection{OrthographicProj(-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 10.0f)};
    RenderVars renderVars{
        model,
        view,
        projection,
        &CustomVertexShader,
        &CustomFragmentShader};

    // Render
    ClearColorBuffer(RGBA{0, 0, 0, 255});
    ClearDepthBuffer(9999.0f);
    Render(tetraMesh, renderVars);

    DrawToConsole();

    return 0;
}