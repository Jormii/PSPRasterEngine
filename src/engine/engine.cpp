#include "engine.hpp"

#include <vector>

#include <pspge.h>
#include <pspvfpu.h>
#include <pspdisplay.h>
#include <psputils.h>

#include "debug.hpp"
#include "constants.hpp"

#include "fragment.hpp"
#include "draw_buffer.hpp"
#include "rasterization.hpp"

#define BUFFER_INDEX(x, y) (x + (PSP_HEIGHT - 1 - y) * PSP_WIDTH)

bool initialized{false};
pspvfpu_context *vfpuContext{nullptr};

DrawMatrices *mat{reinterpret_cast<DrawMatrices *>(SCRATCHPAD_START)};
RGBA *displayBuffer{reinterpret_cast<RGBA *>(0x4000000)};
#if 0
RGBA *drawBuffer{displayBuffer + sizeof(RGBA) * PSP_BUFFER_SIZE};
#else
RGBA *drawBuffer{reinterpret_cast<RGBA *>(0x4300000)};
#endif

// TODO: Can EDRAM be used for the depth buffer?
#if 0
float_psp *depthBuffer{reinterpret_cast<float_psp *>(drawBuffer + sizeof(RGBA) * PSP_BUFFER_SIZE)};
#else
float_psp *depthBuffer{new float_psp[PSP_BUFFER_SIZE]};
#endif

void InitializeContext()
{
    if (initialized)
    {
        return;
    }

    initialized = true;
    vfpuContext = pspvfpu_initcontext();

    // Clear display buffer
    for (size_t i{0}; i < PSP_BUFFER_SIZE; ++i)
    {
        displayBuffer[i] = RGBA{0, 0, 0};
    }

    std::cout << displayBuffer << "\n";
    std::cout << drawBuffer << "\n";
    std::cout << depthBuffer << "\n";

    sceDisplaySetMode(0, PSP_ACTUAL_WIDTH, PSP_HEIGHT);
    sceDisplaySetFrameBuf(displayBuffer, PSP_WIDTH, PSP_DISPLAY_PIXEL_FORMAT_8888, PSP_DISPLAY_SETBUF_IMMEDIATE);
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
    for (size_t i{0}; i < PSP_BUFFER_SIZE; ++i)
    {
        drawBuffer[i] = color;
    }
}

void ClearDepthBuffer(float_psp depth)
{
    for (size_t i{0}; i < PSP_BUFFER_SIZE; ++i)
    {
        depthBuffer[i] = depth;
    }
}

void SwapBuffers()
{
    RGBA *tmp{displayBuffer};
    displayBuffer = drawBuffer;
    drawBuffer = tmp;

    sceDisplaySetFrameBuf(displayBuffer, PSP_WIDTH, PSP_DISPLAY_PIXEL_FORMAT_8888, PSP_DISPLAY_SETBUF_NEXTFRAME);
}

void Draw(const Mesh &mesh, VertexShader vs, FragmentShader fs)
{
    // Vertex shading
    BufferVertexData *buffer{new BufferVertexData[mesh.vertexCount]};

    for (size_t i{0}; i < mesh.vertexCount; ++i)
    {
        Vec4f clipSpace{vs(mesh.vertexData[i], buffer + i)};

        (buffer + i)->position = clipSpace.DivideByW();
        (buffer + i)->viewPos = (mat->mv * Vec4f{mesh.vertexData[i].position, 1.0f}).DivideByW();
        (buffer + i)->uv = mesh.vertexData[i].uv;
    }

    // Rasterize
    DebugStart(DebugIDs::RASTERIZATION);
    InitializeVFPUForRasterization();
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
            drawBuffer[bufferIndex] = RGBA::Vec4fAsRGBA(fsOut.color);
            depthBuffer[bufferIndex] = fsOut.depth;
        }
    }

    DebugEnd(DebugIDs::FRAGMENT_SHADING);

    // Free resources
    delete[] buffer;
}