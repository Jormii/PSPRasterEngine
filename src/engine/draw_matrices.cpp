#include "draw_matrices.hpp"

DrawMatrices::DrawMatrices(const Mat4f &model, const Mat4f &view, const Mat4f &projection)
    : model{model}, view{view}, projection{projection}, mvp{projection * view * model}
{
}