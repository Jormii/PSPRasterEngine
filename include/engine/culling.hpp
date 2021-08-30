#ifndef CULLING_HPP
#define CULLING_HPP

#include "vertex_shading.hpp"

#include "mesh.hpp"

bool *BackfaceCulling(const Mesh &mesh, const VSOut *vsOut);

#endif