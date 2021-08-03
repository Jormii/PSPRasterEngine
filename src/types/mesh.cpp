#include "mesh.hpp"

Mesh::Mesh() : vertices{std::vector<Vec3f>{}}, triangles{std::vector<Vec3i>{}}, color{std::vector<RGBA>{}}
{
}

Mesh::Mesh(std::vector<Vec3f> vertices, std::vector<Vec3i> triangles, std::vector<RGBA> color)
    : vertices{vertices}, triangles{triangles}, color{color}
{
}

Mesh::Mesh(const Mesh &mesh)
    : vertices{mesh.vertices}, triangles{mesh.triangles}, color{mesh.color}
{
}

std::ostream &operator<<(std::ostream &out, const Mesh &mesh)
{
    out << "Mesh: ";
    out << mesh.vertices.size() << " vertices. " << mesh.triangles.size() << " triangles.\n";

    return out;
}