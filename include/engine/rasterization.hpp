#ifndef RASTERIZATION_HPP
#define RASTERIZATION_HPP

#include <vector>

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
 * M300-M500 are grid-dependent
 * 
 * M300 stores Ei(X, Y). (E0(x, y), E1(x, y), E2(x, y)) are rows
 *      (Ei(x, y), Ei(x+1, y), Ei(x, y+1), Ei(x+1, y+1)) are columns
 * 
 * M500 store the barycentric coordinates. (w0, w1, w2, 0) columns
 * 
 * M700 is used for auxiliary operations
 */

void InitializeVFPUForRasterization();
std::vector<Fragment> Rasterize(const Mesh &mesh, const BufferVertexData *buffer);

#endif