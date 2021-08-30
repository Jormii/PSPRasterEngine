#include "vertex_shading.hpp"

VSIn::VSIn(const DrawMatrices &matrices) : matrices{matrices}
{
}

VSOut::VSOut(const Vec4f &vertexTransformed, const RGBA &color)
    : perspectiveDivide{vertexTransformed.DivideByW()}, vertexTransformed{vertexTransformed}, vertexColor{vertexColor}
{
}