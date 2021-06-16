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

bool_psp parallel_to_plane(const Vector3f *p, const Vector3f *q, const Vector3f *clip_plane)
{
    Vector3f pq;
    vector3f_substract(p, q, &pq);

    return vector3f_dot(&pq, clip_plane + 1) == 0.0f;
}

void plane_intersection(const Vector3f *p, const Vector3f *q, const Vector3f *clip_plane, Vector3f *out)
{
    Vector3f p_pplane; // Vector p->plane point
    vector3f_substract(clip_plane, p, &p_pplane);
    float_psp dot_pplane = vector3f_dot(&p_pplane, clip_plane + 1);

    Vector3f pq;
    vector3f_substract(q, p, &pq);
    float_psp dot_pq = vector3f_dot(&pq, clip_plane + 1);

    float_psp t = dot_pplane / dot_pq;
    vector3f_multiply_scalar(&pq, t, out);
    vector3f_add(p, out, out);
}

bool_psp inside_plane(const Vector3f *p, const Vector3f *clip_plane)
{
    Vector3f p_pplane;
    vector3f_substract(p, clip_plane, &p_pplane);

    float_psp dot = vector3f_dot(&p_pplane, clip_plane + 1);
    return dot >= 0;
}