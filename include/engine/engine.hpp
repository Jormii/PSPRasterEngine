#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "vertex_shading.hpp"
#include "fragment_shading.hpp"

#include "types.hpp"
#include "mesh.hpp"
#include "mat4f.hpp"

struct DrawMatrices
{
    Mat4f model;
    Mat4f view;
    Mat4f projection;
    Mat4f mvp;

    DrawMatrices(const Mat4f &model, const Mat4f &view, const Mat4f &projection);
};

void InitializeContext(size_t width, size_t height);
void ClearColorBuffer(const RGBA &color);
void ClearDepthBuffer(float_psp depth);

void Draw(const Mesh &mesh, const DrawMatrices &matrices, VertexShader vs, FragmentShader fs);

#ifndef PSP
void RenderToConsole();
#endif

#endif