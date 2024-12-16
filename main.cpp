#include <cmath>
#include <stdio.h>

#include <pspiofilemgr.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <psppower.h>
#include <pspdisplay.h>

#include "callbacks.hpp"
#include "constants.hpp"
#include "debug.hpp"
#include "engine.hpp"
#include "utils.hpp"
#include "t.hpp"

//#include "./Sample Textures/kass.hpp"
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

void CustomFS(const Fragment &fragment, FSOut &out, const PointLight *light, const Vec3f &lightViewPos)
{
    out.depth = fragment.depth;

    Vec3f v{lightViewPos - fragment.viewPos};
    float_psp d{v.Magnitude()};
    d = (d > 0.01f) ? d : 0.01f;
    float_psp intensity{light->r / d};

    out.color = intensity * light->color;
    // out.color = intensity * kassTexture.fetch(fragment.uv) * light->color;
}

int main()
{
    setupCallbacks();
    InitializeContext();
    scePowerSetClockFrequency(333, 333, 167);

    for (int i=0; i < 10; ++i) {
        Result double_res = double_if_even_result(i);

        printf("Errno: %d\n", (int)double_res.err_code);
        
        if (double_res.ok()) {
            printf("x: %d, 2x: %d\n", i, double_res.value);
        }
    }
    
    printf("%ld\n", __cplusplus);

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

    // Set up lights
    PointLight *light{ActivateLight(0)};
    light->position = Vec3f{3.0f, 1.0f, 0.0f};
    light->color = Vec4f{1.0f, 0.0f, 0.0f, 1.0f};
    light->r = 3.0f;

#if 0
    light = ActivateLight(1);
    light->position = Vec3f{-3.0f, 1.0f, 1.0f};
    light->color = Vec4f{0.0f, 1.0f, 0.0f, 1.0f};
    light->r = 1.5f;
#endif

    do
    {
        angle += 0.1f;
        if (angle >= 2 * M_PI)
        {
            angle -= 2 * M_PI;
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