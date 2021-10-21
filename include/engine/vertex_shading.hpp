#ifndef VERTEX_SHADING_HPP
#define VERTEX_SHADING_HPP

#include "light.hpp"
#include "draw_buffer.hpp"
#include "draw_matrices.hpp"

#include "mesh.hpp"

typedef void (*VertexShader)(const VertexData &vertexData, BufferVertexData *out);

#endif