#include "culling.hpp"

std::vector<Vec3i> BackfaceCulling(std::vector<Vec3i> triangles, std::vector<VSOut> vertexShadingOut)
{
    std::vector<Vec3i> culledTris;
    for (const Vec3i &tri : triangles)
    {
        Vec3f v1{vertexShadingOut[tri.x].vertexView.DivideByW()};
        Vec3f v2{vertexShadingOut[tri.y].vertexView.DivideByW()};
        Vec3f v3{vertexShadingOut[tri.z].vertexView.DivideByW()};

        Vec3f v1v2{v2 - v1};
        Vec3f v2v3{v3 - v2};
        Vec3f faceNormal{Vec3f::Cross(v1v2, v2v3)};

        Vec3f centroid{(v1 + v2 + v3) / 3.0f};
        if (Vec3f::Dot(centroid, faceNormal) < 0)
        {
            culledTris.push_back(tri);
        }
    }

    return culledTris;
}