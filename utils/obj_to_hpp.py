import sys
import os.path

import obj_parser

class RGB:

    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b

    def __repr__(self):
        return "({}, {}, {})".format(self.r, self.g, self.b)

def _create_header(hpp_file):
    hpp_file.write("#include <vector>\n\n")
    hpp_file.write("#include \"vec3f.hpp\"\n")
    hpp_file.write("#include \"vec3i.hpp\"\n")
    hpp_file.write("#include \"rgba.hpp\"\n")
    hpp_file.write("#include \"mesh.hpp\"\n\n")

def _write_vertices(hpp_file, mesh):
    assert mesh.vertices is not None, "Mesh has no vertices"
        
    hpp_file.write("std::vector<Vec3f> {}Vertices{{\n".format(mesh.name))
    for i, vertex in enumerate(mesh.vertices):
        hpp_file.write("\t{}".format(_vec3f_string(vertex)))

        if i != len(mesh.vertices) - 1:
            hpp_file.write(",\n")

    hpp_file.write("};\n\n")

def _write_triangles(hpp_file, mesh):
    assert mesh.triangles is not None, "Mesh has no triangles"

    hpp_file.write("std::vector<Vec3i> {}Triangles{{\n".format(mesh.name))
    for i, triangle in enumerate(mesh.triangles):
        fixed_triangle = obj_parser.Vec3(
            triangle.x - 1,
            triangle.y - 1,
            triangle.z - 1,
            vec_type=int
        )
        hpp_file.write("\t{}".format(_vec3i_string(fixed_triangle)))

        if i != len(mesh.triangles) - 1:
            hpp_file.write(",\n")

    hpp_file.write("};\n\n")

def _write_colors(hpp_file, mesh):
    colors = _get_mesh_colors(mesh)

    hpp_file.write("std::vector<RGBA> {}Color{{\n".format(mesh.name))
    for i, color in enumerate(colors):
        hpp_file.write("\t{}".format(_rgb_string(color)))

        if i != len(colors) - 1:
            hpp_file.write(",\n")

    hpp_file.write("};\n\n")

def _get_mesh_colors(mesh):
    colors = []
    if mesh.colors is None:
        # Paint white
        for i in range(len(mesh.vertices)):
            colors.append(RGB(255, 255, 255))
    else:
        # TODO: Check with actual colors
        pass

    return colors

def _vec3f_string(vector):
    return "Vec3f{{{}f, {}f, {}f}}".format(vector.x, vector.y, vector.z)

def _vec3i_string(vector):
    return "Vec3i{{{}, {}, {}}}".format(vector.x, vector.y, vector.z)

def _rgb_string(rgb):
    return "RGBA{{{}, {}, {}, 255}}".format(rgb.r, rgb.g, rgb.b)

def _write_mesh_initialization(hpp_file, mesh):
    hpp_file.write("Mesh {}Mesh{{{}Vertices, {}Triangles, {}Color}};\n".format(
        mesh.name, mesh.name, mesh.name, mesh.name))

if __name__ == "__main__":
    argc = len(sys.argv)
    if argc == 1:
        exit("No .obj file provided")
    elif argc > 2:
        exit("Expected one argument (target .obj). Got {} instead".format(argc - 1))

    obj_file = sys.argv[1]
    mesh = obj_parser.parse_obj(obj_file)
    mesh.name = mesh.name.lower()

    this_file_abs_path = os.path.dirname(os.path.abspath(__file__))
    hpp_file = open("{}/{}.hpp".format(this_file_abs_path, mesh.name), "w")
    try:
        _create_header(hpp_file)
        _write_vertices(hpp_file, mesh)
        _write_triangles(hpp_file, mesh)
        _write_colors(hpp_file, mesh)
        _write_mesh_initialization(hpp_file, mesh)
    except Exception as e:
        print(e)
    finally:
        hpp_file.close()