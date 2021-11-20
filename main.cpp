#include <cmath>

#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdisplay.h>

#include "callbacks.hpp"
#include "constants.hpp"
#include "debug.hpp"
#include "engine.hpp"
#include "utils.hpp"

#include "./Sample Meshes/plane.hpp"
#include "./Sample Meshes/cube.hpp"

PSP_MODULE_INFO("Engine Test", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

Vec4f CustomVS(const VertexData &vertexData, BufferVertexData *out)
{
    Vec4f vHomo{vertexData.position, 1.0f};
    Vec4f vertexTransformed{Matrices()->mvp * vHomo};

    Vec4f nHomo{vertexData.normal, 0.0f};
    Vec4f nT{Matrices()->n * nHomo};

    out->normal = Vec3f{nT.x, nT.y, nT.z}.Normalize();
    out->color = vertexData.color;
    return vertexTransformed;
}

void CustomFS(const Fragment &fragment, FSOut &out)
{
    out.depth = fragment.depth;

    float_psp dot{Vec3f::Dot(
        -fragment.viewPos.Normalized(),
        fragment.normal.Normalized())};
    out.color = Vec4f{dot, dot, dot, 1.0f};
}

int main()
{
    setupCallbacks();
    InitializeContext();

    // Set up matrices
    float_psp angle{0.25 * M_PI};
    float_psp distance{4.0f};
    float_psp cameraX{distance * cosf(angle)};
    float_psp cameraZ{distance * sinf(angle)};

    Mat4f __attribute__((aligned(16))) model{Mat4f::IDENTITY};
    Mat4f __attribute__((aligned(16))) view{LookAt(
        Vec3f{cameraX, 0.0f, cameraZ},
        Vec3f{0.0f, 0.0f, 0.0f},
        Vec3f{0.0f, 1.0f, 0.0f})};
    Mat4f __attribute__((aligned(16))) projection{PerspectiveProjFov(
        PSP_WIDTH, PSP_HEIGHT,
        40.0f * (M_PI / 180),
        1.0f, 10.0f)};

    SceCtrlData pad;
    do
    {
        // Read input
        sceCtrlReadBufferPositive(&pad, 1);
        if (pad.Buttons & PSP_CTRL_CROSS)
        {
            break;
        }

        if (pad.Buttons & PSP_CTRL_SQUARE)
        {
            angle += 0.1f;
        }
        if (pad.Buttons & PSP_CTRL_CIRCLE)
        {
            angle -= 0.1;
        }

        if (angle >= 2 * M_PI)
        {
            angle -= 2 * M_PI;
        }
        else if (angle < 0)
        {
            angle += 2 * M_PI;
        }

        // Update view matrix and matrixes
        cameraX = distance * cosf(angle);
        cameraZ = distance * sinf(angle);
        view = LookAt(
            Vec3f{cameraX, 0.0f, cameraZ},
            Vec3f{0.0f, 0.0f, 0.0f},
            Vec3f{0.0f, 1.0f, 0.0f});

        Matrices()->Init(model, view, projection);

        ClearColorBuffer(RGBA{122, 122, 255});
        ClearDepthBuffer(9999.0f);

        Draw(cubeMesh, &CustomVS, &CustomFS);
        Draw(planeMesh, &CustomVS, &CustomFS);

        // Print debug to console
        PrintDebugDataAndReset();

        SwapBuffers();
        sceDisplayWaitVblankStart();
    } while (running());

    DestroyContext();
    sceKernelExitGame();

    return 0;
}