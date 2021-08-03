#include "vertex_shading.hpp"

VSIn::VSIn(const Mat4f &model, const Mat4f &view, const Mat4f &proj)
    : model{model}, view{view}, projection{proj},
      modelView{view * model}, modelViewProj{proj * view * model}
{
}

VSOut::VSOut(const Vec4f &vertexView, const Vec4f &vertexHomo)
    : vertexView{vertexView}, vertexHomo{vertexHomo}, vertexPerspDiv{vertexHomo.DivideByW()}
{
}