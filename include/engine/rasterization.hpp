#ifndef RASTERIZATION_HPP
#define RASTERIZATION_HPP

#include <vector>

#include "constants.hpp"
#include "draw_buffer.hpp"
#include "fragment.hpp"

#include "mesh.hpp"
#include "vec2f.hpp"
#include "vec3f.hpp"

/***
 * NOTES
 * M000 stores constants. Layout:
 *      (   HW      HH      0       1   )
 *      (   HW      HH      0       1   )
 *      (   HW      HH      0       1   )
 *      (  WMax    HMax     0       1   )
 * 
 * where:   HW, HH are the half width and half height of the screen
 *          WMax, HMax are the maximum screen coordinates -1
 * 
 * M100 stores the screen coordinates (3x2 matrix starting in S100)
 * 
 * M200 stores the edge function's variables a, b, c in a 3x3 matrix where
 *      (b, a, c, a+b)i are columns. a AND b ARE SWAPPED
 * 
 * M300-M600 are grid-dependent
 * 
 * M300 stores Ei(X, Y). (E0(x, y), E1(x, y), E2(x, y)) are columns
 *      (Ei(x, y), Ei(x+1, y), Ei(x, y+1), Ei(x+1, y+1)) are rows
 * 
 * M300's fourth row stores if the pixel is within a triangle
 * 
 * M400 store the barycentric coordinates. (w0, w1, w2, 0) columns
 * 
 * M500 and M600 are used for interpolation
 * 
 * M700 is used for auxiliary operations
 */

#define GRID_SIZE 2

constexpr float_psp VFPU_ALIGN GRID_SIZE_VEC4[]{GRID_SIZE, GRID_SIZE, GRID_SIZE, GRID_SIZE};

constexpr float_psp HALF_SCREEN_VEC2[]{
    0.5f * static_cast<float_psp>(PSP_WIDTH),
    0.5f * static_cast<float_psp>(PSP_HEIGHT)};

constexpr float_psp MAX_SCREEN_VEC2[]{
    static_cast<float_psp>(PSP_WIDTH - 1.0f),
    static_cast<float_psp>(PSP_HEIGHT - 1.0f)};

void InitializeVFPUForRasterization();
std::vector<Fragment> Rasterize(const Mesh &mesh, const BufferVertexData *buffer);

#endif