#ifndef CLIPPING_PLANE_H
#define CLIPPING_PLANE_H

#define N_CLIPPING_PLANES 6

#include "../types/vector3f.h"

// Arranged as pairs (point, normal)
const Vector3f FRUSTUM_PLANES[2 * N_CLIPPING_PLANES];

bool_psp inside_plane(const Vector3f *p, const Vector3f *clip_plane);
bool_psp parallel_to_plane(const Vector3f *p, const Vector3f *q, const Vector3f *clip_plane);
Vector3f plane_intersection(const Vector3f *p, const Vector3f *q, const Vector3f *clip_plane);

#endif