#include <cmath>

#ifdef PSP
#include <pspgu.h>
#include <pspctrl.h>
#include <pspkernel.h>
#endif

#include "engine.hpp"
#include "utils.hpp"

#include "./Sample Meshes/plane.hpp"
#include "./Sample Meshes/tetra.hpp"
#include "./Sample Meshes/cube.hpp"

#ifdef PSP
constexpr size_t WIDTH{512};
constexpr size_t HEIGHT{272};

PSP_MODULE_INFO("Engine Test", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);
#elif REDIRECT
constexpr size_t WIDTH{140};
constexpr size_t HEIGHT{35};
#else
constexpr size_t WIDTH{48};
constexpr size_t HEIGHT{14};
#endif

void CustomVS(const DrawMatrices &matrices, const VertexData &vertexData, BufferVertexData *out, const bool *activeLights, const PointLight *lights)
{
    Vec4f vHomo{vertexData.position, 1.0f};
    Vec4f vertexTransformed{matrices.mvp * vHomo};

    Vec4f nHomo{vertexData.normal, 0.0f};
    Vec4f nT{matrices.normalMat * nHomo};

    out->positionHomo = vertexTransformed;
    out->normal = Vec3f{nT.x, nT.y, nT.z}.Normalize();
    out->color = vertexData.color;
}

void CustomFS(const DrawMatrices &matrices, const Fragment &fragment, FSOut &out, const bool *activeLights, const PointLight *lights)
{
    out.depth = fragment.depth;

    Vec4f outColor;
    for (size_t i{0}; i < N_LIGHTS; ++i)
    {
        if (!activeLights[i])
        {
            continue;
        }

        const PointLight &light{lights[i]};
        Vec3f lPos{(matrices.view * Vec4f{light.position, 1.0f}).DivideByW()};
        Vec3f l{lPos - fragment.viewPos};
        float_psp norm{l.Magnitude()};

        float_psp r{std::max(0.001f, sqrtf(norm * norm))};
        Vec3f l_u{l / r};

        float_psp dot{Vec3f::Dot(fragment.normal, l_u)};

        float_psp ratio{light.r / r};
        Vec4f shade{ratio * ratio * light.color};
        outColor = outColor + dot * shade * fragment.color;
    }

    out.color = outColor;
}

int main()
{
#ifdef PSP
    // Needed for now
    pspDebugScreenInit();
#endif

    InitializeContext(WIDTH, HEIGHT);

    // Set up matrices
    float_psp angle{0.25 * M_PI};
    float_psp distance{4.0f};
    float_psp cameraX{distance * cosf(angle)};
    float_psp cameraZ{distance * sinf(angle)};

    Mat4f model{Mat4f::IDENTITY};
    Mat4f view{LookAt(
        Vec3f{cameraX, 0.0f, cameraZ},
        Vec3f{0.0f, 0.0f, 0.0f},
        Vec3f{0.0f, 1.0f, 0.0f})};
    Mat4f projection{PerspectiveProjFov(WIDTH, HEIGHT, 40.0f * (M_PI / 180), 1.0f, 10.0f)};

    DrawMatrices matrices{model, view, projection};

    // Set up lights
    PointLight *light;
    light = ActivateLight(0);
    light->position = Vec3f{1.5f, 0.0f, 1.5f};
    light->color = Vec4f{1.0f, 1.0f, 0.0f, 1.0f};
    light->r = 1.0f;

    light = ActivateLight(1);
    light->position = Vec3f{2.0f, 1.0f, -0.5f};
    light->color = Vec4f{0.0f, 1.0f, 1.0f, 1.0f};
    light->r = 1.5f;

    // Draw call
    ClearColorBuffer(RGBA{0, 0, 0}); // Black
    ClearDepthBuffer(9999.0f);
    Draw(tetraMesh, matrices, &CustomVS, &CustomFS);
    Draw(planeMesh, matrices, &CustomVS, &CustomFS);

#ifdef PSP
    SceCtrlData pad;
    while (true)
    {
        sceCtrlReadBufferPositive(&pad, 1);
        if (pad.Buttons & PSP_CTRL_CROSS)
        {
            break;
        }
    }
    sceKernelExitGame();
#else
    RenderToConsole();
#endif

    return 0;
}