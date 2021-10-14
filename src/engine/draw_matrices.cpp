#include "draw_matrices.hpp"

DrawMatrices::DrawMatrices(const Mat4f &model, const Mat4f &view, const Mat4f &projection)
    : model{model}, view{view}, projection{projection}, mv{view * model}, mvp{projection * mv}
{
    Mat4f mv{view * model};
    if (!mv.Inverse(normalMat))
    {
        exit(-1); // TODO
    }

    normalMat = normalMat.Transpose();
}