#ifndef RASTERIZATION_HPP
#define RASTERIZATION_HPP

#include <vector>

#include "draw_buffer.hpp"
#include "fragment.hpp"

#include "mesh.hpp"
#include "vec2f.hpp"
#include "vec3f.hpp"

std::vector<Fragment> Rasterize(const Mesh &mesh, const BufferVertexData *buffer, size_t width, size_t height);
Vec2f VertexScreenCoordinate(const Vec3f &p, size_t width, size_t height);

#endif