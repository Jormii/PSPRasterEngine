#include "vertex_shading.hpp"

VSIn::VSIn(const Mat4f &model, const Mat4f &view, const Mat4f &proj)
    : model{model}, view{view}, projection{proj},
      modelView{view * model}, modelViewProj{proj * view * model}
{
}

VSOut::VSOut(const Vec4f &vertexMVP, const Vec4f &vertexHomo)
    : vertexMVP{vertexMVP}, vertexHomo{vertexHomo}, vertexPerspDiv{vertexMVP.DivideByW()}
{
}