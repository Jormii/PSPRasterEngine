#include "clipping.hpp"

// Clip Plane implementation

ClipPlane::ClipPlane(const Vec3f &point, const Vec3f &normal) : point{point}, normal{normal}
{
}

bool ClipPlane::PointInPositiveSpace(const Vec3f &p) const
{
    Vec3f v{p - point};
    return Vec3f::Dot(v, normal) >= 0.0f;
}

bool ClipPlane::LineIsParallel(const Vec3f &p, const Vec3f &q) const
{
    Vec3f pq{q - p};
    return Vec3f::Dot(pq, normal) == 0.0f;
}

Vec3f ClipPlane::LineIntersection(const Vec3f &p, const Vec3f &q) const
{
    Vec3f pq{q - p};
    float_psp t{
        Vec3f::Dot((point - p), normal) /
        Vec3f::Dot(pq, normal)};

    return Vec3f{p + t * pq};
}

std::ostream &operator<<(std::ostream &out, const ClipPlane &plane)
{
    out << "P: " << plane.point << ", N: " << plane.normal;
    return out;
}

// Clipping.hpp functions

Mesh Clip(const Mesh &mesh, std::vector<VSOut> vertexShadingOut, const std::vector<Vec3i> culledTriangles)
{
    Mesh outMesh;
    int_psp triOffset{0};

    for (const Vec3i &tri : culledTriangles)
    {
        Mesh clippedTri{ClipTriangle(tri.x, tri.y, tri.z, mesh, vertexShadingOut)};
        for (Vec3i &clipTri : clippedTri.triangles)
        {
            clipTri.x += triOffset;
            clipTri.y += triOffset;
            clipTri.z += triOffset;
        }

        outMesh.vertices.insert(outMesh.vertices.end(), clippedTri.vertices.begin(), clippedTri.vertices.end());
        outMesh.triangles.insert(outMesh.triangles.end(), clippedTri.triangles.begin(), clippedTri.triangles.end());

        triOffset += clippedTri.vertices.size();
    }

    return outMesh;
}

Mesh ClipTriangle(int_psp a, int_psp b, int_psp c, const Mesh &mesh, std::vector<VSOut> vertexShadingOut)
{
    // Sutherland–Hodgman algorithm
    std::vector<Vec3f> outputVertices{
        vertexShadingOut[a].vertexPerspDiv,
        vertexShadingOut[b].vertexPerspDiv,
        vertexShadingOut[c].vertexPerspDiv};

    for (const ClipPlane &clipPlane : FRUSTUM_PLANES)
    {
        std::vector<Vec3f> vertices{outputVertices};
        outputVertices.clear();

        for (size_t i{0}; i < vertices.size(); ++i)
        {
            Vec3f &current{vertices[i]};
            Vec3f &next{vertices[(i + 1) % vertices.size()]};

            bool currentInside{clipPlane.PointInPositiveSpace(current)};
            bool nextInside{clipPlane.PointInPositiveSpace(next)};

            // Ignore parallel
            if (clipPlane.LineIsParallel(current, next))
            {
                if (currentInside)
                {
                    outputVertices.push_back(current);
                }
                continue;
            }

            Vec3f intersection{clipPlane.LineIntersection(current, next)};
            if (currentInside)
            {
                outputVertices.push_back(current);
                if (!nextInside)
                {
                    outputVertices.push_back(intersection);
                }
            }
            else if (nextInside)
            {
                outputVertices.push_back(intersection);
            }
        }
    }

    // Triangulate
    // 0 is the common vertex
    std::vector<Vec3i> outputTriangles;
    for (int_psp i{0}; i < static_cast<int_psp>(outputVertices.size()) - 2; ++i)
    {
        outputTriangles.push_back(Vec3i{0, i + 1, i + 2});
    }

    std::vector<RGBA> outputColor{outputVertices.size()}; // TODO
    return Mesh{outputVertices, outputTriangles, outputColor};
}