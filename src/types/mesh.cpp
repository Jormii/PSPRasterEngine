#include "mesh.hpp"

VertexData::VertexData(const Vec3f &position, const RGBA &color) : position{position}, color{color}
{
}

Mesh::Mesh(size_t vertexCount, size_t triangleCount, VertexData *vertexData, Vec3i *triangles)
    : vertexCount{vertexCount}, triangleCount{triangleCount}, vertexData{vertexData}, triangles{triangles}
{
}

std::ostream &operator<<(std::ostream &out, const Mesh &mesh)
{
    out << "Mesh: ";
    out << mesh.vertexCount << " vertices. " << mesh.triangleCount << " triangles.\n";

    return out;
}