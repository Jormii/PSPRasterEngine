from plyfile import PlyData

from mesh import Mesh
from vector import Vec2, Vec3

import os


def parse_ply(ply_file):
    mesh = Mesh("ply_mesh")
    plydata = PlyData.read(ply_file)

    _write_vertices(mesh, plydata)
    _write_normals(mesh, plydata)
    _write_colors(mesh, plydata)
    _write_uvs(mesh, plydata)
    _write_triangles(mesh, plydata)

    return mesh


def _write_vertices(mesh, plydata):
    vertex_data = plydata["vertex"]
    X = vertex_data["x"]
    Y = vertex_data["y"]
    Z = vertex_data["z"]

    vertex_count = len(X)
    for i in range(vertex_count):
        mesh.vertices.append(Vec3(X[i], Y[i], Z[i]))


def _write_normals(mesh, plydata):
    vertex_data = plydata["vertex"]
    Nx = vertex_data["nx"]
    Ny = vertex_data["ny"]
    Nz = vertex_data["nz"]

    vertex_count = len(Nx)
    for i in range(vertex_count):
        mesh.normals.append(Vec3(Nx[i], Ny[i], Nz[i]))


def _write_colors(mesh, plydata):
    vertex_data = plydata["vertex"]
    vertex_count = len(vertex_data["x"])

    if "red" not in vertex_data:
        # No color information. Paint white
        white = Vec3(255, 255, 255, vec_type=int)
        for i in range(vertex_count):
            mesh.colors.append(white)
    else:
        R = vertex_data["red"]
        G = vertex_data["green"]
        B = vertex_data["blue"]

        for i in range(vertex_count):
            mesh.colors.append(Vec3(R[i], G[i], B[i], vec_type=int))


def _write_uvs(mesh, plydata):
    vertex_data = plydata["vertex"]

    U = vertex_data["s"]
    V = vertex_data["t"]
    for u, v in zip(U, V):
        mesh.uvs.append(Vec2(u, v))


def _write_triangles(mesh, plydata):
    triangles = plydata["face"].data
    for tri_obj in triangles:
        tri = tri_obj[0]
        assert len(tri) == 3, "Non-triangular faces"

        mesh.triangles.append(Vec3(*tri, vec_type=int))
