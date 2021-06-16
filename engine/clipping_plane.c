#include "./clipping_plane.h"

const Vector3f FRUSTUM_PLANES[] = {
    {-1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f},
    {0.0f, -1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, -1.0f, 0.0f},
    {0.0f, 0.0f, -1.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, -1.0f}};

bool_psp inside_plane(const Vector3f *p, const Vector3f *clip_plane)
{
    Vector3f p_pplane = vector3f_substract(p, clip_plane);
    return vector3f_dot(&p_pplane, clip_plane + 1) >= 0;
}

bool_psp parallel_to_plane(const Vector3f *p, const Vector3f *q, const Vector3f *clip_plane)
{
    Vector3f pq = vector3f_substract(p, q);
    return vector3f_dot(&pq, clip_plane + 1) == 0.0f;
}

Vector3f plane_intersection(const Vector3f *p, const Vector3f *q, const Vector3f *clip_plane)
{
    Vector3f p_pplane = vector3f_substract(clip_plane, p);
    Vector3f pq = vector3f_substract(q, p);
    float_psp t = vector3f_dot(&p_pplane, clip_plane + 1) / vector3f_dot(&pq, clip_plane + 1);

    Vector3f intersection = vector3f_multiply_scalar(&pq, t);
    intersection = vector3f_add(p, &intersection);
    return intersection;
}