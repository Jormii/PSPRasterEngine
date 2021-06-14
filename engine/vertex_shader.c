#include "./vertex_shader.h"

#include <stdio.h>

void vs_out_print(VertexShaderOut *vs_out)
{
    vector4f_print(&vs_out->clip_coord);
    printf("\t");
    rgba_print(&vs_out->color);
}