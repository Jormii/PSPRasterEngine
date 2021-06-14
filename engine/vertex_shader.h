#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

#include "../types/vector3f.h"
#include "../types/vector4f.h"
#include "../types/matrix4f.h"
#include "../types/rgba.h"

typedef struct VertexShaderIn_t
{
    const void *vertex_vector;

    const Matrix4f *model;
    const Matrix4f *view;
    const Matrix4f *projection;
    const Matrix4f *model_view;
    const Matrix4f *model_view_projection;
} VertexShaderIn;

typedef struct VertexShaderOut_t
{
    Vector4f clip_coord;
    RGBA color;
} VertexShaderOut;

typedef void (*VertexShader)(const VertexShaderIn *input, VertexShaderOut *output);

void vs_out_print(VertexShaderOut *vs_out);

#endif