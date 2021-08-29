#include <vector>

#include "./include/engine/engine.hpp"
#include "./include/engine/utils.hpp"

// #define REDIRECT
#ifdef REDIRECT
constexpr int_psp WIDTH{140};
constexpr int_psp HEIGHT{38};
#else
constexpr int_psp WIDTH{48};
constexpr int_psp HEIGHT{24};
#endif

VSOut CustomVertexShader(const VSIn &vsIn)
{
    Vec4f vHomo{vsIn.vertex, 1.0f};
    Vec4f vertexMVP{vsIn.modelViewProj * vHomo};

    std::cout << vertexMVP.DivideByW() << "\n";

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

    // Create mesh
    std::vector<Vec3f> vertices{
        Vec3f{-1.0f, 1.0f, 1.0f},
        Vec3f{1.0f, 1.0f, 1.0f},
        Vec3f{1.0f, 1.0f, -1.0f},
        Vec3f{-1.0f, 1.0f, -1.0f},
        Vec3f{-1.0f, -1.0f, 1.0f},
        Vec3f{1.0f, -1.0f, 1.0f},
        Vec3f{1.0f, -1.0f, -1.0f},
        Vec3f{-1.0f, -1.0f, -1.0f}};

    std::vector<Vec3i> triangles{
        Vec3i{0, 4, 1},
        Vec3i{1, 4, 5},
        Vec3i{1, 5, 2},
        Vec3i{2, 5, 6},
        Vec3i{2, 6, 3},
        Vec3i{3, 6, 7},
        Vec3i{3, 7, 0},
        Vec3i{0, 7, 4},
        Vec3i{0, 1, 2},
        Vec3i{0, 2, 3},
        Vec3i{4, 7, 5},
        Vec3i{5, 7, 6}};

    std::vector<RGBA> color;
    for (size_t i{0}; i < vertices.size(); ++i)
    {
        color.push_back(RGBA{255, 255, 255, 255});
    }

    Mesh mesh{vertices, triangles, color};

    // Create render variables
    Mat4f model{Mat4f::IDENTITY};
    Mat4f view{LookAt(
        Vec3f{2.0f, 1.0f, 2.0f},
        Vec3f{0.0f, 0.0f, 0.0f},
        Vec3f{0.0f, 1.0f, 0.0f})};
    Mat4f projection{PerspectiveProjFov(WIDTH, HEIGHT, 30.0f, 1.0f, 10.0f)};
    RenderVars renderVars{
        model,
        view,
        projection,
        &CustomVertexShader,
        &CustomFragmentShader};

    std::cout << renderVars.projection << "\n\n";

    // Render
    ClearColorBuffer(RGBA{0, 0, 0, 255});
    ClearDepthBuffer(9999.0f);
    Render(mesh, renderVars);

    DrawToConsole();

    return 0;
}