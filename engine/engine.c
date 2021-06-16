#include "./engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./vertex_shader.h"
#include "./clipping_plane.h"

typedef struct VertexData_t
{
    Vector3f position;
    Vector3f normal;
    RGBA color;
    Vector2f uv;
} VertexData;

// Basic context
bool_psp initialized;
uint_psp viewport_width;
uint_psp viewport_height;
uint_psp buffer_size;

// Draw buffers
RGBA *color_buffer;
float_psp *depth_buffer;

// Mesh buffer
void *mesh_buffer;

struct VertexShadingOutput
{
    VertexShaderOut *buffer;
} vs_out;

struct PerspectiveDivideOutput
{
    Vector3f *buffer;
} persp_div_out;

struct CullingOutput
{
    uint_psp visible_vertices;
    uint_psp visible_triangles;
    bool_psp *visible;
} cull_out;

struct ClippingOutput
{
    uint_psp vertex_count;
    uint_psp triangle_count;
    VertexShaderOut *buffer;
    Vector3i *triangles;
} clip_out;

static void vertex_shading(const DrawingInput *input, uint_psp vertex_count, uint_psp vector_size);
static void perspective_divide(uint_psp vertex_count);
static void backface_culling(uint_psp vertex_count, const Vector3i *triangles, uint_psp tri_count);
static void clipping(uint_psp vertex_count, const Vector3i *triangles, uint_psp tri_count);
static void clip_triangle(const Vector3f *a, const Vector3f *b, const Vector3f *c);

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
    vertex_shading(input, vertex_count, vector_size);
    perspective_divide(vertex_count);
    backface_culling(vertex_count, triangles, tri_count);
    clipping(vertex_count, triangles, tri_count);

    /// Free resources
    free(vs_out.buffer);
    free(persp_div_out.buffer);
    free(cull_out.visible);
    free(clip_out.buffer);
}

static void vertex_shading(const DrawingInput *input, uint_psp vertex_count, uint_psp vector_size)
{
    Matrix4f model_view;
    Matrix4f model_view_projection;
    matrix4f_matrix_mult(input->view, input->model, &model_view);
    matrix4f_matrix_mult(input->projection, &model_view, &model_view_projection);

    VertexShaderIn shading_in = {
        0,
        input->model,
        input->view,
        input->projection,
        &model_view,
        &model_view_projection};
    VertexShaderOut shading_out;

    // Perform vertex shading
    vs_out.buffer = malloc(vertex_count * sizeof(VertexShaderOut));
    for (uint_psp i = 0; i < vertex_count; ++i)
    {
        shading_in.vertex_vector = mesh_buffer + i * vector_size;
        input->vs(&shading_in, &shading_out);
        memcpy(vs_out.buffer + i, &shading_out, sizeof(VertexShaderOut));
    }
}

static void perspective_divide(uint_psp vertex_count)
{
    persp_div_out.buffer = malloc(vertex_count * sizeof(Vector3f));
    for (uint_psp i = 0; i < vertex_count; ++i)
    {
        vector4f_divide_by_w(&(vs_out.buffer + i)->clip_coord, persp_div_out.buffer + i);
    }
}

static void backface_culling(uint_psp vertex_count, const Vector3i *triangles, uint_psp tri_count)
{
    Vector3f eye = {0.0f, 0.0f, -1.0f};
    cull_out.visible = malloc(tri_count * sizeof(bool_psp));
    for (uint_psp i = 0; i < tri_count; ++i)
    {
        Vector3i tri = triangles[i];
        Vector3f a = persp_div_out.buffer[tri.x];
        Vector3f b = persp_div_out.buffer[tri.y];
        Vector3f c = persp_div_out.buffer[tri.z];

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
        cull_out.visible[i] = vector3f_dot(&eye_bary, &normal) < 0;
    }

    // TODO
    cull_out.visible_vertices = vertex_count;
    cull_out.visible_triangles = tri_count;
}

static void clipping(uint_psp vertex_count, const Vector3i *triangles, uint_psp tri_count)
{
    // Initialize buffers
    uint_psp max_vertex = 2 * vertex_count;
    uint_psp max_triangles = 6 * tri_count;
    clip_out.buffer = malloc(max_vertex * sizeof(VertexShaderOut));
    clip_out.triangles = malloc(max_triangles * sizeof(Vector3i));
    clip_out.vertex_count = 0;
    clip_out.triangle_count = 0;

    // Perform clipping
    for (uint_psp i = 0; i < tri_count; ++i)
    {
        // Don't process backfacing triangles
        if (!cull_out.visible)
        {
            continue;
        }

        Vector3i tri = triangles[i];
        clip_triangle(
            persp_div_out.buffer + tri.x,
            persp_div_out.buffer + tri.y,
            persp_div_out.buffer + tri.z);
    }
}

static void clip_triangle(const Vector3f *a, const Vector3f *b, const Vector3f *c)
{
    // Sutherland–Hodgman algorithm

    // Initialize
    uint_psp max_vertices = 6; // At worst every vertex is divided in two
    Vector3f output[max_vertices];
    memcpy(output, a, sizeof(Vector3f));
    memcpy(output + 1, b, sizeof(Vector3f));
    memcpy(output + 2, c, sizeof(Vector3f));
    Vector3f vertices[max_vertices];
    uint8_psp n_vertices = 3;

    // Perform algoritm
    for (uint_psp i = 0; i < N_CLIPPING_PLANES; ++i)
    {
        const Vector3f *clipping_plane = FRUSTUM_PLANES + i * 2;
        uint8_psp n_new_vertices = 0;
        memcpy(vertices, output, n_vertices * sizeof(Vector3f));

        for (uint_psp j = 0; j < n_vertices; ++i)
        {
            Vector3f *current = vertices + j;
            Vector3f *next = vertices + ((j + 1) % n_vertices);

            uint8_psp current_inside = inside_plane(current, clipping_plane);
            uint8_psp next_inside = inside_plane(next, clipping_plane);

            // Ignore if parallel to plane
            if (parallel_to_plane(current, next, clipping_plane))
            {
                if (current_inside)
                {
                    memcpy(output + n_new_vertices, current, sizeof(Vector3f));
                    n_new_vertices += 1;
                }
                continue;
            }

            // Find intersection
            Vector3f intersection;
            plane_intersection(current, next, clipping_plane, &intersection);
            if (current_inside)
            {
                memcpy(output + n_new_vertices, current, sizeof(Vector3f));
                n_new_vertices += 1;

                if (!next_inside)
                {
                    memcpy(output + n_new_vertices, &intersection, sizeof(Vector3f));
                }
            }
            else if (next_inside)
            {
                memcpy(output + n_new_vertices, &intersection, sizeof(Vector3f));
            }
        }

        n_vertices = n_new_vertices;
    }
}