#include "engine.hpp"

#include <vector>

#include <pspdebug.h>
#include <pspvfpu.h>

#include "constants.hpp"
#include "debug.hpp"
#include "fragment.hpp"
#include "draw_buffer.hpp"
#include "rasterization.hpp"

#define BUFFER_INDEX(x, y) (x + (PSP_HEIGHT - 1 - y) * PSP_WIDTH)

bool initialized{false};
pspvfpu_context *vfpuContext{0};
DrawMatrices *mat{reinterpret_cast<DrawMatrices *>(SCRATCHPAD_START)};
RGBA *colorBuffer{reinterpret_cast<RGBA *>(0x04000000)};
float_psp *depthBuffer{new float_psp[PSP_BUFFER_SIZE]};

void InitializeContext()
{
    if (initialized)
    {
        return;
    }

    initialized = true;
    pspDebugScreenInit(); // Needed (for now at least)
    pspDebugScreenClearLineDisable();
    vfpuContext = pspvfpu_initcontext();
}

void DestroyContext()
{
    if (!initialized)
    {
        return;
    }

    initialized = false;
    pspvfpu_deletecontext(vfpuContext);
}

DrawMatrices *Matrices()
{
    return mat;
}

void ClearColorBuffer(const RGBA &color)
{
    DebugStart(DebugIDs::CLEAR_COLOR_BUFFER);
    for (size_t i{0}; i < PSP_BUFFER_SIZE; ++i)
    {
        colorBuffer[i] = color;
    }
    DebugEnd(DebugIDs::CLEAR_COLOR_BUFFER);
}

void ClearDepthBuffer(float_psp depth)
{
    DebugStart(DebugIDs::CLEAR_DEPTH_BUFFER);
    for (size_t i{0}; i < PSP_BUFFER_SIZE; ++i)
    {
        depthBuffer[i] = depth;
    }
    DebugEnd(DebugIDs::CLEAR_DEPTH_BUFFER);
}

void Draw(const Mesh &mesh, VertexShader vs, FragmentShader fs)
{
    // Vertex shading
    DebugStart(DebugIDs::BUFFER_ALLOCATION);
    BufferVertexData *buffer{new BufferVertexData[mesh.vertexCount]};
    DebugEnd(DebugIDs::BUFFER_ALLOCATION);

    DebugStart(DebugIDs::VERTEX_SHADING);
    for (size_t i{0}; i < mesh.vertexCount; ++i)
    {
        vs(mesh.vertexData[i], buffer + i);

        (buffer + i)->position = (buffer + i)->positionHomo.DivideByW();
        (buffer + i)->viewPos = (mat->mv * Vec4f{mesh.vertexData[i].position, 1.0f}).DivideByW();
        (buffer + i)->uv = mesh.vertexData[i].uv;
    }
    DebugEnd(DebugIDs::VERTEX_SHADING);

    // Rasterize
    DebugStart(DebugIDs::RASTERIZATION);
    std::vector<Fragment> fragments{Rasterize(mesh, buffer)};
    DebugEnd(DebugIDs::RASTERIZATION);

    // Fragment shading
    DebugStart(DebugIDs::FRAGMENT_SHADING);
    FSOut fsOut;
    for (const Fragment &fragment : fragments)
    {
        fs(fragment, fsOut);

        size_t bufferIndex{BUFFER_INDEX(fragment.xScreenCoord, fragment.yScreenCoord)};
        if (fsOut.depth < depthBuffer[bufferIndex])
        {
            colorBuffer[bufferIndex] = RGBA::Vec4fAsRGBA(fsOut.color);
            depthBuffer[bufferIndex] = fsOut.depth;
        }
    }
    DebugEnd(DebugIDs::FRAGMENT_SHADING);

    // Free resources
    DebugStart(DebugIDs::BUFFER_FREE);
    delete[] buffer;
    DebugEnd(DebugIDs::BUFFER_FREE);
}