#ifndef VERTEX_SHADING_HPP
#define VERTEX_SHADING_HPP

#include "engine.hpp"

#include "vec3f.hpp"
#include "vec4f.hpp"
#include "mat4f.hpp"

struct VSIn
{
    Vec3f vertex;
    RGBA color;
    const DrawMatrices &matrices;

    VSIn(const DrawMatrices &matrices);
};

struct VSOut
{
    Vec3f perspectiveDivide;
    Vec4f vertexTransformed;
    RGBA vertexColor;

    VSOut();
    VSOut(const Vec4f &vertexTransformed, const RGBA &color);
};

typedef VSOut (*VertexShader)(const VSIn &input);

#endif