#include "culling.hpp"

bool *BackfaceCulling(const Mesh &mesh, const VSOut *vsOut)
{
    bool *trisToDraw{new bool[mesh.triangleCount]};
    for (size_t i{0}; i < mesh.triangleCount; ++i)
    {
        const Vec3i *tri{mesh.triangles + i};
        Vec3f v1{vsOut[tri->x].perspectiveDivide};
        Vec3f v2{vsOut[tri->y].perspectiveDivide};
        Vec3f v3{vsOut[tri->z].perspectiveDivide};

        Vec3f v1v2{v2 - v1};
        Vec3f v2v3{v3 - v2};
        Vec3f faceNormal{Vec3f::Cross(v1v2, v2v3)};

        Vec3f centroid{(v1 + v2 + v3) / 3.0f};
        trisToDraw[i] = Vec3f::Dot(centroid, faceNormal) >= 0;
    }

    return trisToDraw;
}