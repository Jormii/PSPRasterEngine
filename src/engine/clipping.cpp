#include "clipping.hpp"
#include "utils.hpp"

struct ClipPlane
{
    Vec3f point;
    Vec3f normal;

    ClipPlane(const Vec3f &point, const Vec3f &normal);

    bool PointInPositiveSpace(const Vec3f &p) const;
    bool LineIsParallel(const Vec3f &p, const Vec3f &q) const;
    Tuple<Vec3f, float_psp> LineIntersection(const Vec3f &p, const Vec3f &q) const;

    friend std::ostream &operator<<(std::ostream &out, const ClipPlane &plane);
};

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

Tuple<Vec3f, float_psp> ClipPlane::LineIntersection(const Vec3f &p, const Vec3f &q) const
{
    Vec3f pq{q - p};
    float_psp t{
        Vec3f::Dot((point - p), normal) /
        Vec3f::Dot(pq, normal)};

    return Tuple<Vec3f, float_psp>{Vec3f{p + t * pq}, t};
}

std::ostream &operator<<(std::ostream &out, const ClipPlane &plane)
{
    out << "P: " << plane.point << ", N: " << plane.normal;
    return out;
}

/* Frustum's clipping planes
In order: left, right, bottom, top, near and far */
const ClipPlane FRUSTUM_PLANES[]{
    ClipPlane(Vec3f(-1.0f, 0.0f, 0.0f), Vec3f(1.0f, 0.0f, 0.0f)),
    ClipPlane(Vec3f(1.0f, 0.0f, 0.0f), Vec3f(-1.0f, 0.0f, 0.0f)),
    ClipPlane(Vec3f(0.0f, -1.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f)),
    ClipPlane(Vec3f(0.0f, 1.0f, 0.0f), Vec3f(0.0f, -1.0f, 0.0f)),
    ClipPlane(Vec3f(0.0f, 0.0f, -1.0f), Vec3f(0.0f, 0.0f, 1.0f)),
    ClipPlane(Vec3f(0.0f, 0.0f, 1.0f), Vec3f(0.0f, 0.0f, -1.0f))};

void ClipTriangle(const Vec3i &tri, const VSOut *vsOut, Mesh &clippedMesh, Vec4f *verticesHomogeneous);

ClipOut::ClipOut(const Mesh &mesh, const Vec4f *verticesHomogeneous) : clippedMesh(mesh), verticesHomogeneous{verticesHomogeneous}
{
}

ClipOut Clip(const Mesh &mesh, const VSOut *vsOut, const bool *trisToDraw)
{
    // Initialize output
    size_t maxVertexCount{9 * mesh.triangleCount};
    size_t maxTriangleCount{(9 - 2) * mesh.triangleCount};
    Mesh clippedMesh{0, 0, new VertexData[maxVertexCount], new Vec3i[maxTriangleCount]};
    Vec4f *verticesHomogeneous{new Vec4f[maxVertexCount]};

    for (size_t i{0}; i < mesh.triangleCount; ++i)
    {
        if (!trisToDraw[i])
        {
            continue;
        }

        ClipTriangle(mesh.triangles[i], vsOut, clippedMesh, verticesHomogeneous);
    }

    return ClipOut{clippedMesh, verticesHomogeneous};
}

void ClipTriangle(const Vec3i &tri, const VSOut *vsOut, Mesh &clippedMesh, Vec4f *verticesHomogeneous)
{
    // Sutherland–Hodgman algorithm
    // Copy initial data
    size_t nOutputVertices{3};
    clippedMesh.vertexData[clippedMesh.vertexCount].position = vsOut[tri.x].perspectiveDivide;
    clippedMesh.vertexData[clippedMesh.vertexCount].color = vsOut[tri.x].vertexColor;
    verticesHomogeneous[clippedMesh.vertexCount] = vsOut[tri.x].vertexTransformed;

    clippedMesh.vertexData[clippedMesh.vertexCount + 1].position = vsOut[tri.y].perspectiveDivide;
    clippedMesh.vertexData[clippedMesh.vertexCount + 1].color = vsOut[tri.y].vertexColor;
    verticesHomogeneous[clippedMesh.vertexCount + 1] = vsOut[tri.y].vertexTransformed;

    clippedMesh.vertexData[clippedMesh.vertexCount + 2].position = vsOut[tri.z].perspectiveDivide;
    clippedMesh.vertexData[clippedMesh.vertexCount + 2].color = vsOut[tri.z].vertexColor;
    verticesHomogeneous[clippedMesh.vertexCount + 2] = vsOut[tri.z].vertexTransformed;

    for (const ClipPlane &clipPlane : FRUSTUM_PLANES)
    {
        // Copy output data to new mesh
        Mesh tmpMesh{nOutputVertices, 0, new VertexData[nOutputVertices], nullptr}; // Triangles not needed
        Vec4f *tmpVertHomo{new Vec4f[nOutputVertices]};
        for (size_t i{0}; i < tmpMesh.vertexCount; ++i)
        {
            tmpMesh.vertexData[i] = clippedMesh.vertexData[clippedMesh.vertexCount + i];
            tmpVertHomo[i] = verticesHomogeneous[clippedMesh.vertexCount + i];
        }

        nOutputVertices = 0;

        for (size_t i{0}; i < tmpMesh.vertexCount; ++i)
        {
            VertexData &currentData{tmpMesh.vertexData[i]};
            VertexData &nextData{tmpMesh.vertexData[(i + 1) % tmpMesh.vertexCount]};

            bool currentInside{clipPlane.PointInPositiveSpace(currentData.position)};
            bool nextInside{clipPlane.PointInPositiveSpace(nextData.position)};

            // Ignore parallel
            if (clipPlane.LineIsParallel(currentData.position, nextData.position))
            {
                if (currentInside)
                {
                    clippedMesh.vertexData[i + nOutputVertices] = currentData;
                    verticesHomogeneous[i + nOutputVertices] = tmpVertHomo[i];
                    nOutputVertices += 1;
                }

                continue;
            }

            Tuple<Vec3f, float_psp> intersectionResult{clipPlane.LineIntersection(currentData.position, nextData.position)};
            if (currentInside)
            {
                clippedMesh.vertexData[i + nOutputVertices] = currentData;
                verticesHomogeneous[i + nOutputVertices] = tmpVertHomo[i];
                nOutputVertices += 1;

                if (!nextInside)
                {
                    clippedMesh.vertexData[i + nOutputVertices].position = intersectionResult.first;

                    float_psp t{intersectionResult.second};
                    clippedMesh.vertexData[i + nOutputVertices].color = RGBA::Interpolate(
                        currentData.color, nextData.color, t);
                    verticesHomogeneous[i + nOutputVertices] = Vec4f::Interpolate(
                        tmpVertHomo[i], tmpVertHomo[(i + 1) % tmpMesh.vertexCount], t);

                    nOutputVertices += 1;
                }
                else if (nextInside)
                {
                    clippedMesh.vertexData[i + nOutputVertices].position = intersectionResult.first;

                    float_psp t{intersectionResult.second};
                    clippedMesh.vertexData[i + nOutputVertices].color = RGBA::Interpolate(
                        currentData.color, nextData.color, t);
                    verticesHomogeneous[i + nOutputVertices] = Vec4f::Interpolate(
                        tmpVertHomo[i], tmpVertHomo[(i + 1) % tmpMesh.vertexCount], t);

                    nOutputVertices += 1;
                }
            }
        }

        // Free acquired resources
        delete tmpMesh.vertexData;
        delete tmpVertHomo;
    }

    // Triangulate
    for (size_t i{0}; i < nOutputVertices - 2; ++i)
    {
        clippedMesh.triangles[i + clippedMesh.triangleCount] = Vec3i{
            clippedMesh.triangleCount, clippedMesh.triangleCount + i + 1, clippedMesh.triangleCount + i + 2};
    }

    // Update mesh
    clippedMesh.vertexCount += nOutputVertices;
    clippedMesh.triangleCount += nOutputVertices - 2;
}