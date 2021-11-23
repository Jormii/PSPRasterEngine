#ifndef RASTERIZATION_HPP
#define RASTERIZATION_HPP

#include <vector>

#include "draw_buffer.hpp"
#include "fragment.hpp"

#include "mesh.hpp"
#include "vec2f.hpp"
#include "vec3f.hpp"

void InitializeVFPUForRasterization();
std::vector<Fragment> Rasterize(const Mesh &mesh, const BufferVertexData *buffer);

#endif