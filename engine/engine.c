#include "./engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./vertex_shader.h"

// Basic context
int initialized;
uint_psp viewport_width;
uint_psp viewport_height;
uint_psp buffer_size;

// Draw buffers
RGBA *color_buffer;
float_psp *depth_buffer;

// Mesh buffer
void *mesh_buffer;

void initialize_context(uint_psp width, uint_psp height)
{
    if (initialized)
    {
        return;
    }

    initialized = 1;
    viewport_width = width;
    viewport_height = height;
    buffer_size = width * height;

    color_buffer = malloc(buffer_size * sizeof(RGBA));
    depth_buffer = malloc(buffer_size * sizeof(float_psp));

    mesh_buffer = 0;
}

void alloc_buffer(uint_psp count, uint_psp size)
{
    if (mesh_buffer != 0)
    {
        free_buffer();
    }

    mesh_buffer = malloc(count * size);
}

void free_buffer()
{
    if (mesh_buffer != 0)
    {
        free(mesh_buffer);
    }
}

void buffer_data(const void *data, uint_psp vertex_count, uint_psp instance_size, uint_psp vector_size, uint_psp offset)
{
    for (uint_psp i = 0; i < vertex_count; ++i)
    {
        void *destination = mesh_buffer + i * vector_size + offset;
        const void *source = data + i * instance_size;
        memcpy(destination, source, instance_size);
    }
}

void draw_triangles(const DrawingInput *input, uint_psp vertex_count, uint_psp vector_size, const Vector3i *triangles, uint_psp tri_count)
{
    /// Vertex shading
    // Initialize variables
    void *vertex_buffer = malloc(vertex_count * sizeof(VertexShaderOut));

    Matrix4f model_view;
    Matrix4f model_view_projection;
    matrix4f_matrix_mult(input->view, input->model, &model_view);
    matrix4f_matrix_mult(input->projection, &model_view, &model_view_projection);

    VertexShaderIn vs_input = {
        0,
        input->model,
        input->view,
        input->projection,
        &model_view,
        &model_view_projection};
    VertexShaderOut vs_out;

    // Perform vertex shading
    for (uint_psp i = 0; i < vertex_count; ++i)
    {
        vs_input.vertex_vector = mesh_buffer + i * vector_size;
        input->vs(&vs_input, &vs_out);
        memcpy(vertex_buffer + i * sizeof(VertexShaderOut), &vs_out, sizeof(VertexShaderOut));
    }

    for (uint_psp i=0; i < vertex_count; ++i) {
        void *vector = vertex_buffer + i*sizeof(VertexShaderOut);
        vector4f_print(vector);
        printf("\t");
        vector4f_print(vector + sizeof(Vector4f));
        printf("\t");
        rgba_print(vector + 2*sizeof(Vector4f));
        printf("\n");
    }

    // Free resources
    free(vertex_buffer);
}