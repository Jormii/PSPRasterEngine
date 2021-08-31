#include "engine.hpp"

#include "culling.hpp"
#include "clipping.hpp"
#include "fragment.hpp"
#include "rasterization.hpp"

struct EngineContext
{
    bool initialized{false};

    size_t viewportWidth;
    size_t viewportHeight;
    size_t bufferSize;

    RGBA *colorBuffer;
    float_psp *depthBuffer;
} context;

DrawMatrices::DrawMatrices(const Mat4f &model, const Mat4f &view, const Mat4f &projection)
    : model{model}, view{view}, projection{projection}, mvp{projection * view * model}
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

void Draw(const Mesh &mesh, const DrawMatrices &matrices, VertexShader vs, FragmentShader fs)
{
    // Vertex shading
    VSOut *vsOut{new VSOut[mesh.vertexCount]};
    VSIn vsIn{matrices};

    for (size_t i{0}; i < mesh.vertexCount; ++i)
    {
        const VertexData *vertexData{mesh.vertexData + i};
        vsIn.vertex = vertexData->position;
        vsIn.color = vertexData->color;

        vsOut[i] = vs(vsIn);
    }

    // Backface culling
    bool *trisToDraw{BackfaceCulling(mesh, vsOut)};

    // Clipping
    ClipOut clippingOut{Clip(mesh, vsOut, trisToDraw)};

    // Rasterization
    std::vector<Fragment> fragments{Rasterize(clippingOut, context.viewportWidth, context.viewportHeight)};

    // Fragment shader
    FSIn fsIn;
    for (const Fragment &fragment : fragments)
    {
        fsIn.pixel.x = fragment.xScreenCoord;
        fsIn.pixel.y = fragment.yScreenCoord;
        fsIn.depth = fragment.depth;
        fsIn.color = fragment.color;

        FSOut fsOut{vars.fs(fsIn)};

        size_t bufferIndex{fragment.xScreenCoord +
                           (context.viewportHeight - 1 - fragment.yScreenCoord) * context.viewportWidth};
        if (fsOut.depth < context.depthBuffer[bufferIndex])
        {
            context.colorBuffer[bufferIndex] = fsOut.color;
            context.depthBuffer[bufferIndex] = fsOut.depth;
        }
    }

    // Free resources
    delete[] vsOut;
    delete[] trisToDraw;
    delete[] clippedMesh.clippedMesh.vertexData;
    delete[] clippedMesh.clippedMesh.triangles;
    delete[] clippedMesh.verticesHomogeneous;
}

#ifndef PSP
void RenderToConsole()
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