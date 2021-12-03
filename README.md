# PSP Raster Engine

This was a project I carried to learn C++ and 3D computer graphics, done for the PSP because I love the system and additionally the system provides tighter constraints.

The goal was simple: [The PSP's hardware carries 3D operations using a specific chipset](http://hitmen.c02.at/files/yapspd/psp_doc/chap11.html#sec11). 
However, this piece of hardware has a sort of "fixed behaviour", meaning that it doesn't allow custom shaders (as far as I know). This program's purpose was to
provide a complete 3D pipeline (excluding the clipping step) without using the 3D chipset the PSP provides.

The whole pipeline of the program goes as follows: Vertex shader -> Backface culling + Rasterization -> Fragment shading.

## Remarkable aspects
- Fully working pipeline, providing full control over the shading process.
- Rasterization, save triangle traversal, uses the [PSP's VFPU (Vector Floating Point Unit)](http://hitmen.c02.at/files/yapspd/psp_doc/chap4.html#sec4) entirely.
- Pixels are rasterized at once using 2x2 grids [traversed by zig-zagging](https://oa.upm.es/9184/1/INVE_MEM_2010_84947.pdf).

![Program sample](https://i.imgur.com/AyTd9Y6.png)

## Problems found
- PSP's display and draw buffer's memory addressed aren't uncached. Writing to them was extremely slow.
- Rasterization is a huge bottleneck even though the process uses the VFPU almost entirely. The reason for the bottleneck must be another like cache misses.
- All in all, the program struggles to reach 20 fps rendering only a cube and a plane.

## Abandoned project
While I wanted to keep working on the program, the above problems make it hard to make it feasible to end with a working real-time software. Even though,
the following aspects are left to implement:
- Other types of lights. Only point clouds are included.
- Bump mapping.
- Shadows and occlusions.
