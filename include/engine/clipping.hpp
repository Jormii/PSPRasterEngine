#ifndef CLIPPING_HPP
#define CLIPPING_HPP

#include <vector>

#include "vertex_shading.hpp"
#include "clipping.hpp"

#include "mesh.hpp"

#include "types.hpp"
#include "vec3i.hpp"
#include "vec3f.hpp"

struct ClipPlane
{
    Vec3f point;
    Vec3f normal;

    ClipPlane(const Vec3f &point, const Vec3f &normal);

    bool PointInPositiveSpace(const Vec3f &p) const;
    bool LineIsParallel(const Vec3f &p, const Vec3f &q) const;
    Vec3f LineIntersection(const Vec3f &p, const Vec3f &q) const;

    friend std::ostream &operator<<(std::ostream &out, const ClipPlane &plane);
};

/* Frustum's clipping planes
In order: left, right, bottom, top, near and far */
const ClipPlane FRUSTUM_PLANES[]{
    ClipPlane(Vec3f(-1.0f, 0.0f, 0.0f), Vec3f(1.0f, 0.0f, 0.0f)),
    ClipPlane(Vec3f(1.0f, 0.0f, 0.0f), Vec3f(-1.0f, 0.0f, 0.0f)),
    ClipPlane(Vec3f(0.0f, -1.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f)),
    ClipPlane(Vec3f(0.0f, 1.0f, 0.0f), Vec3f(0.0f, -1.0f, 0.0f)),
    ClipPlane(Vec3f(0.0f, 0.0f, -1.0f), Vec3f(0.0f, 0.0f, 1.0f)),
    ClipPlane(Vec3f(0.0f, 0.0f, 1.0f), Vec3f(0.0f, 0.0f, -1.0f))};

Mesh Clip(const Mesh &mesh, std::vector<VSOut> vertexShadingOut, const std::vector<Vec3i> culledTriangles);
Mesh ClipTriangle(int_psp a, int_psp b, int_psp c, const Mesh &mesh, const std::vector<VSOut> vertexShadingOut);

#endif