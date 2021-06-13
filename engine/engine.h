#ifndef ENGINE_H
#define ENGINE_H

#include "./vertex_shader.h"

#include "../types/types.h"
#include "../types/vector3i.h"
#include "../types/vector3f.h"
#include "../types/matrix4f.h"
#include "../types/rgba.h"

typedef struct DrawingInput_t
{
    const Matrix4f *model;
    const Matrix4f *view;
    const Matrix4f *projection;

    VertexShader vs;
} DrawingInput;

void initialize_context(uint_psp width, uint_psp height);

void alloc_buffer(uint_psp count, uint_psp size);
void free_buffer();
void buffer_data(const void *data, uint_psp vertex_count, uint_psp instance_size, uint_psp vector_size, uint_psp offset);

void draw_triangles(const DrawingInput *input, uint_psp vertex_count, uint_psp vector_size, const Vector3i *triangles, uint_psp tri_count);

#endif