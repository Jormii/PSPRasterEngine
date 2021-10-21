#include "draw_matrices.hpp"

#include "debug.hpp"

void DrawMatrices::Init(const Mat4f &m, const Mat4f &v, const Mat4f &p)
{
    // Load matrixes in VFPU
    // Model
    asm(
        "lv.q C000, 0(%0);"
        "lv.q C010, 16(%0);"
        "lv.q C020, 32(%0);"
        "lv.q C030, 48(%0)"
        :
        : "r"(&m)
        :);

    // View
    asm(
        "lv.q C100, 0(%0);"
        "lv.q C110, 16(%0);"
        "lv.q C120, 32(%0);"
        "lv.q C130, 48(%0)"
        :
        : "r"(&v)
        :);

    // Projection
    asm(
        "lv.q C200, 0(%0);"
        "lv.q C210, 16(%0);"
        "lv.q C220, 32(%0);"
        "lv.q C230, 48(%0)"
        :
        : "r"(&p)
        :);

    // Do computations
    // M000: Model
    // M100: View
    // M200: Projection
    asm(
        "vmmul.q M300, M000, M100;" // mv
        "vmmul.q M400, M300, M200;" // mvp
    );

    // Store
    // M300: ModelView
    // M400: ModelViewProjection
    asm(
        "sv.q C300, 0(%0);" // mv
        "sv.q C310, 16(%0);"
        "sv.q C320, 32(%0);"
        "sv.q C330, 48(%0);"
        "sv.q C400, 0(%1);" // mvp
        "sv.q C410, 16(%1);"
        "sv.q C420, 32(%1);"
        "sv.q C430, 48(%1);"
        :
        : "r"(&mv), "r"(&mvp)
        :);

    // TODO: Normal
    if (!mv.Inverse(n))
    {
        exit(-1); // TODO: Do something better
    }
    n = n.Transpose();
}