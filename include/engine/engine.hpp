#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "light.hpp"
#include "vertex_shading.hpp"
#include "fragment_shading.hpp"

#include "types.hpp"
#include "mesh.hpp"
#include "rgba.hpp"

constexpr size_t N_LIGHTS{2};

void InitializeContext(size_t width, size_t height);
void ClearColorBuffer(const RGBA &color);
void ClearDepthBuffer(float_psp depth);

PointLight *ActivateLight(size_t index);
void DeactivateLight(size_t index);

void Draw(const Mesh &mesh, const DrawMatrices &matrices, VertexShader vs, FragmentShader fs);

#ifndef PSP
void RenderToConsole();
#endif

#endif