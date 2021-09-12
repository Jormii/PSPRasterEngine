import sys
import os.path

import obj_parser
import ply_parser
from vector import Vec3


def _write_guard_and_includes(hpp_file, mesh):
    define = "{}_HPP".format(mesh.name).upper()

    hpp_file.write("#ifndef {}\n".format(define))
    hpp_file.write("#define {}\n\n".format(define))
    hpp_file.write("#include \"mesh.hpp\"\n\n")


def _write_vertex_data(hpp_file, mesh):
    assert mesh.vertices is not None, "Mesh has no vertices"

    vertices = mesh.vertices
    normals = mesh.normals
    colors = _get_mesh_colors(mesh)

    for data in [normals, colors]:
        assert len(vertices) == len(data), "Vertex count mismatch"

    hpp_file.write("VertexData {}VertexData[]{{\n".format(mesh.name))
    for i in range(len(vertices)):
        v = vertices[i]
        n = normals[i]
        c = colors[i]

        hpp_file.write("\tVertexData{{{}, {}, {}}}".format(
            _vec3f_string(v), _vec3f_string(n), _rgba_string(c)))
        if i != len(mesh.vertices) - 1:
            hpp_file.write(",\n")

    hpp_file.write("};\n\n")


def _get_mesh_colors(mesh):
    colors = []
    if mesh.colors is None:
        # Paint white
        for i in range(len(mesh.vertices)):
            colors.append(Vec3(255, 255, 255, vec_type=int))
    else:
        colors = mesh.colors

    return colors


def _write_triangles(hpp_file, mesh):
    assert mesh.triangles is not None, "Mesh has no triangles"

    hpp_file.write("Vec3i {}Triangles[]{{\n".format(mesh.name))
    for i, triangle in enumerate(mesh.triangles):
        hpp_file.write("\t{}".format(_vec3i_string(triangle)))

        if i != len(mesh.triangles) - 1:
            hpp_file.write(",\n")

    hpp_file.write("};\n\n")


def _vec3f_string(vector):
    return "Vec3f{{{}f, {}f, {}f}}".format(vector.x, vector.y, vector.z)


def _vec3i_string(vector):
    return "Vec3i{{{}, {}, {}}}".format(vector.x, vector.y, vector.z)


def _rgba_string(vector):
    return "RGBA{{{}, {}, {}, 255}}".format(vector.x, vector.y, vector.z)


def _write_mesh_initialization(hpp_file, mesh):
    name = mesh.name
    nVertices = len(mesh.vertices)
    nTris = len(mesh.triangles)

    hpp_file.write("Mesh {}Mesh{{{}, {}, {}VertexData, {}Triangles}};\n".format(
        name, nVertices, nTris, name, name))


PARSING_FUNC = {
    "obj": obj_parser.parse_obj,
    "ply": ply_parser.parse_ply
}

if __name__ == "__main__":
    argc = len(sys.argv)
    if argc == 1:
        exit("No mesh file provided")
    elif argc > 2:
        exit("Expected one argument (target mesh). Got {} instead".format(argc - 1))

    mesh_file = sys.argv[1]
    extension = mesh_file.split(".")[-1]
    try:
        mesh = PARSING_FUNC[extension](mesh_file)
        mesh.name = mesh.name.lower()
    except KeyError:
        print("Unknown mesh format .{}".format(extension))

    this_file_abs_path = os.path.dirname(os.path.abspath(__file__))
    hpp_file = open("{}/{}.hpp".format(this_file_abs_path, mesh.name), "w")
    try:
        _write_guard_and_includes(hpp_file, mesh)
        _write_vertex_data(hpp_file, mesh)
        _write_triangles(hpp_file, mesh)
        _write_mesh_initialization(hpp_file, mesh)

        hpp_file.write("\n#endif")
    except Exception as e:
        print(e)
    finally:
        hpp_file.close()
