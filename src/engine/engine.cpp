#include "engine.hpp"

#include <vector>

#include "fragment.hpp"
#include "draw_buffer.hpp"
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

size_t BufferIndex(size_t x, size_t y);

size_t BufferIndex(size_t x, size_t y)
{
    return x + (context.viewportHeight - 1 - y) * context.viewportWidth;
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
    BufferVertexData *buffer{new BufferVertexData[mesh.vertexCount]};
    for (size_t i{0}; i < mesh.vertexCount; ++i)
    {
        vs(matrices, mesh.vertexData[i], buffer + i);
        (buffer + i)->position = (buffer + i)->positionHomo.DivideByW();
    }

    // Rasterize
    std::vector<Fragment> fragments{Rasterize(mesh, buffer, context.viewportWidth, context.viewportHeight)};

    // Fragment shading
    FSOut fsOut;
    for (const Fragment &fragment : fragments)
    {
        fs(fragment, fsOut);

        size_t bufferIndex{BufferIndex(fragment.xScreenCoord, fragment.yScreenCoord)};
        if (fsOut.depth < context.depthBuffer[bufferIndex])
        {
            context.colorBuffer[bufferIndex] = fsOut.color;
            context.depthBuffer[bufferIndex] = fsOut.depth;
        }
    }

    // Free resources
    delete[] buffer;
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