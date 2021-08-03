#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <vector>

#include "vertex_shading.hpp"
#include "fragment_shading.hpp"

#include "mesh.hpp"

#include "types.hpp"
#include "mat4f.hpp"

struct RenderVars
{
    Mat4f model;
    Mat4f view;
    Mat4f projection;
    VertexShader vs;
    FragmentShader fs;

    RenderVars(const Mat4f &model, const Mat4f &view, const Mat4f &projection, VertexShader vs, FragmentShader fs);
};

void InitializeContext(size_t width, size_t height);
void ClearColorBuffer(const RGBA &rgb);
void ClearDepthBuffer(float_psp depth);

void Render(const Mesh &mesh, const RenderVars &vars);

#ifndef PSP
void DrawToConsole();
#endif

#endif