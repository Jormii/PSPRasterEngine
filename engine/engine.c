#include "./engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./vertex_shader.h"

// Basic context
uint8_psp initialized;
uint_psp viewport_width;
uint_psp viewport_height;
uint_psp buffer_size;

// Draw buffers
RGBA *color_buffer;
float_psp *depth_buffer;

// Mesh buffer
void *mesh_buffer;

static VertexShaderOut *vertex_shading(const DrawingInput *input, uint_psp vertex_count, uint_psp vector_size);
static uint8_psp *backface_culling(uint_psp vertex_count, const Vector3i *triangles, uint_psp tri_count, VertexShaderOut *vs_buff);

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
    // Vertex shading
    VertexShaderOut *vs_buff = vertex_shading(input, vertex_count, vector_size);

    // Backface culling
    uint8_psp *culling_buffer = backface_culling(vertex_count, triangles, tri_count, vs_buff);

    /// Free resources
    free(vs_buff);
    free(culling_buffer);
}

static VertexShaderOut *vertex_shading(const DrawingInput *input, uint_psp vertex_count, uint_psp vector_size)
{
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
    VertexShaderOut *vs_buffer = malloc(vertex_count * sizeof(VertexShaderOut));
    for (uint_psp i = 0; i < vertex_count; ++i)
    {
        vs_input.vertex_vector = mesh_buffer + i * vector_size;
        input->vs(&vs_input, &vs_out);
        memcpy(vs_buffer + i, &vs_out, sizeof(VertexShaderOut));
    }

    return vs_buffer;
}

static uint8_psp *backface_culling(uint_psp vertex_count, const Vector3i *triangles, uint_psp tri_count, VertexShaderOut *vs_buff)
{
    // Perspective divide
    Vector3f *divided_by_w = malloc(vertex_count * sizeof(Vector3f));
    for (uint_psp i = 0; i < vertex_count; ++i)
    {
        vector4f_divide_by_w(&(vs_buff + i)->clip_coord, divided_by_w + i);
    }

    // Backface culling
    Vector3f eye = {0.0f, 0.0f, -1.0f};
    uint8_psp *culling_buffer = malloc(tri_count * sizeof(uint8_psp));
    for (uint_psp i = 0; i < tri_count; ++i)
    {
        Vector3i tri = triangles[i];
        Vector3f a = divided_by_w[tri.x];
        Vector3f b = divided_by_w[tri.y];
        Vector3f c = divided_by_w[tri.z];

        // Obtain triangle normal
        Vector3f ab, ac;
        vector3f_substract(&b, &a, &ab);
        vector3f_substract(&c, &a, &ac);

        Vector3f normal;
        vector3f_cross(&ab, &ac, &normal);

        // Calculate barycenter
        Vector3f barycenter;
        vector3f_add(&a, &b, &barycenter);
        vector3f_add(&c, &barycenter, &barycenter);
        vector3f_divide_scalar(&barycenter, 3.0f, &barycenter);

        // Check if frontfacing
        Vector3f eye_bary;
        vector3f_substract(&barycenter, &eye, &eye_bary);
        culling_buffer[i] = vector3f_dot(&eye_bary, &normal) < 0;
    }

    // Free resources
    free(divided_by_w);

    return culling_buffer;
}