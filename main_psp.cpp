#include <cmath>

#include <pspgu.h>
#include <pspctrl.h>
#include <pspkernel.h>

#include "engine.hpp"
#include "utils.hpp"

#include "types.hpp"

#include "./Sample Meshes/tetra.hpp"

PSP_MODULE_INFO("Engine Test", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

constexpr size_t PSP_SCREEN_WIDTH{512};
constexpr size_t PSP_SCREEN_HEIGHT{272};

void CustomVS(const DrawMatrices &matrices, const VertexData &vertexData, BufferVertexData *out)
{
    Vec4f vHomo{vertexData.position, 1.0f};
    Vec4f vertexTransformed{matrices.mvp * vHomo};

    Vec4f nHomo{vertexData.normal, 0.0f};
    Vec4f nT{matrices.normalMat * nHomo};

    out->positionHomo = vertexTransformed;
    out->normal = Vec3f{nT.x, nT.y, nT.z}.Normalize();
    out->color = vertexData.color;

    // Out color
    Vec3f v{(matrices.mv * vHomo).DivideByW().Normalize()};
    float_psp dot{Vec3f::Dot(-v, out->normal)};
    uint8_psp dotUint{std::max(
        static_cast<uint8_psp>(0),
        static_cast<uint8_psp>(255.0f * dot))};

    out->color = RGBA(dotUint, dotUint, dotUint);
}

void CustomFS(const Fragment &fragment, FSOut &out)
{
    out.depth = fragment.depth;
    out.color = fragment.color;
}

int main()
{
    // Needed for now
    pspDebugScreenInit();

    InitializeContext(PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT);

    // Set up matrices
    float_psp angle{0.25 * M_PI};
    float_psp distance{3.5f};
    float_psp cameraX{distance * cosf(angle)};
    float_psp cameraZ{distance * sinf(angle)};

    Mat4f model{Mat4f::IDENTITY};
    Mat4f view{LookAt(
        Vec3f{cameraX, 0.0f, cameraZ},
        Vec3f{0.0f, 0.0f, 0.0f},
        Vec3f{0.0f, 1.0f, 0.0f})};
    Mat4f projection{PerspectiveProjFov(PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 60.0f * (M_PI / 180), 1.0f, 10.0f)};

    DrawMatrices matrices{model, view, projection};

    // Draw call
    ClearColorBuffer(RGBA{0, 0, 0}); // Black
    ClearDepthBuffer(9999.0f);
    Draw(tetraMesh, matrices, &CustomVS, &CustomFS);

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

    return 0;
}