#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "vertex_shading.hpp"
#include "fragment_shading.hpp"

#include "types.hpp"
#include "mesh.hpp"
#include "rgba.hpp"

void InitializeContext(size_t width, size_t height);
void ClearColorBuffer(const RGBA &color);
void ClearDepthBuffer(float_psp depth);

void Draw(const Mesh &mesh, const DrawMatrices &matrices, VertexShader vs, FragmentShader fs);

#ifndef PSP
void RenderToConsole();
#endif

#endif