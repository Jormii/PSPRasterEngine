#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "constants.hpp"
#include "light.hpp"
#include "vertex_shading.hpp"
#include "fragment_shading.hpp"

#include "types.hpp"
#include "mesh.hpp"
#include "rgba.hpp"

#define N_LIGHTS 2

void InitializeContext();
void DestroyContext();
DrawMatrices *Matrices();
void ClearColorBuffer(const RGBA &color);
void ClearDepthBuffer(float_psp depth);
void SwapBuffers();
PointLight *ActivateLight(size_t id);
void DeactivateLight(size_t id);
void Draw(const Mesh &mesh, VertexShader vs, FragmentShader fs);

#endif