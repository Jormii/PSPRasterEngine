#ifndef VERTEX_SHADING_HPP
#define VERTEX_SHADING_HPP

#include "vec3f.hpp"
#include "vec4f.hpp"
#include "mat4f.hpp"

struct VSIn
{
    Vec3f vertex;
    const Mat4f &model;
    const Mat4f &view;
    const Mat4f &projection;
    const Mat4f modelView;
    const Mat4f modelViewProj;

    VSIn(const Mat4f &model, const Mat4f &view, const Mat4f &proj);
};

struct VSOut
{
    Vec4f vertexMVP;
    Vec4f vertexHomo;
    Vec3f vertexPerspDiv;

    VSOut(const Vec4f &vertexMVP, const Vec4f &vertexHomo);
};

typedef VSOut (*VertexShader)(const VSIn &input);

#endif