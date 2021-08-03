#include "engine.hpp"

#include "vertex_shading.hpp"
#include "culling.hpp"
#include "clipping.hpp"
#include "fragment.hpp"
#include "rasterization.hpp"

#include "types.hpp"
#include "rgba.hpp"

struct EngineContext
{
    bool initialized{false};

    size_t viewportWidth;
    size_t viewportHeight;
    size_t bufferSize;

    RGBA *colorBuffer;
    float_psp *depthBuffer;

    RGBA clearColorBuffer{0, 0, 0, 255};
    float_psp clearDepthBuffer{9999.9f};
} context;

RenderVars::RenderVars(const Mat4f &model, const Mat4f &view, const Mat4f &projection, VertexShader vs, FragmentShader fs)
    : model{model}, view{view}, projection{projection}, vs{vs}, fs{fs}
{
}

void InitializeContext(size_t width, size_t height)
{
    if (context.initialized)
    {
        return;
    }

    context.initialized = true;

    context.viewportWidth = width;
    context.viewportHeight = height;
    context.bufferSize = width * height;

#ifndef PSP
    context.colorBuffer = new RGBA[context.bufferSize];
    context.depthBuffer = new float_psp[context.bufferSize];
#else
    context.colorBuffer = reinterpret_cast<RGBA *>(0x04000000);
    context.depthBuffer = new float_psp[context.bufferSize];
#endif
}

void ClearColorBuffer(const RGBA &color)
{
    for (size_t i{0}; i < context.bufferSize; ++i)
    {
        context.colorBuffer[i] = color;
    }
}

void ClearDepthBuffer(float_psp depth)
{
    for (size_t i{0}; i < context.bufferSize; ++i)
    {
        context.depthBuffer[i] = depth;
    }
}

void Render(const Mesh &mesh, const RenderVars &vars)
{
    // Vertex shading
    std::vector<VSOut> vsOut;
    VSIn vsIn{vars.model, vars.view, vars.projection};

    for (const Vec3f &vertex : mesh.vertices)
    {
        vsIn.vertex = vertex;
        vsOut.push_back(vars.vs(vsIn));
    }

    // Backface culling
    std::vector<Vec3i> culledTriangles{BackfaceCulling(mesh.triangles, vsOut)};

    // Clipping
    Mesh clippingOut{Clip(mesh, vsOut, culledTriangles)};

    // Rasterization
    std::vector<Fragment> fragments{Rasterize(clippingOut, context.viewportWidth, context.viewportHeight)};

    // Fragment shader
    FSIn fsIn;
    for (const Fragment &fragment : fragments)
    {
        fsIn.pixel = Vec2i{fragment.xScreenCoord, fragment.yScreenCoord};
        fsIn.depth = fragment.depth;

        FSOut fsOut{vars.fs(fsIn)};

        size_t bufferIndex{fragment.xScreenCoord +
                           (context.viewportHeight - 1 - fragment.yScreenCoord) * context.viewportWidth};
        if (fsOut.depth < context.depthBuffer[bufferIndex])
        {
            context.colorBuffer[bufferIndex] = fsOut.color;
            context.depthBuffer[bufferIndex] = fsOut.depth;
        }
    }
}

#ifndef PSP
void DrawToConsole()
{
    for (size_t i{0}; i < context.viewportWidth; ++i)
    {
        std::cout << "-";
    }
    std::cout << "\n";
    for (size_t i{0}; i < context.bufferSize; ++i)
    {
        std::cout << ((context.colorBuffer[i]) ? "x" : " ");

        if ((i + 1) % context.viewportWidth == 0)
        {
            std::cout << "\n";
        }
    }
    for (size_t i{0}; i < context.viewportWidth; ++i)
    {
        std::cout << "-";
    }
    std::cout << "\n";
}
#endif