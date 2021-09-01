from mesh import Mesh
from vector import Vec2, Vec3

# .obj fields
OBJ_COMMENT_FIELD = "#"
OBJ_MESH_NAME_FIELD = "o"
OBJ_VERTEX_FIELD = "v"
OBJ_UV_FIELD = "vt"
OBJ_NORMAL_FIELD = "vn"
OBJ_SMOOTH_SHADING_FIELD = "s"
OBJ_FACE_DATA_FIELD = "f"

OBJ_FACE_DATA_VALID_FIELDS = [
    OBJ_UV_FIELD,
    OBJ_NORMAL_FIELD
]


def parse_obj(obj_path):
    mesh = Mesh("obj_mesh")
    field_order = []

    file = open(obj_path, "r")
    line = file.readline()
    line_count = 1

    try:
        while line:
            _parse_line(line, mesh, field_order)
            line = file.readline()
            line_count += 1
    except Exception as e:
        print("Unknown field. Exception caused by line {}".format(line_count))
        print(e)
    finally:
        file.close()

    return mesh


def _parse_line(line, mesh, field_order):
    if len(line) == 0:
        # Ignore empty lines
        return

    line_split = line.strip().split(" ")
    field = line_split[0]
    if field in OBJ_FACE_DATA_VALID_FIELDS and field not in field_order:
        field_order.append(field)

    PARSE_FUNCS[field](line_split, mesh, field_order)


def _parse_comment(split_line, mesh, field_order):
    return


def _parse_mesh_name(split_line, mesh, field_order):
    mesh_name = split_line[1]
    mesh.name = mesh_name


def _parse_vertex(split_line, mesh, field_order):
    coordinates = split_line[1:]
    assert len(coordinates) == 3

    mesh.vertices.append(Vec3(*coordinates))


def _parse_uv(split_line, mesh, field_order):
    uvs = split_line[1:]
    assert len(uvs) == 2

    mesh.uvs.append(Vec2(*uvs))


def _parse_normal(split_line, mesh, field_order):
    normals = split_line[1:]
    assert len(normals) == 3

    mesh.normals.append(Vec3(*normals))


def _parse_smooth(split_line, mesh, field_order):
    # TODO
    return


def _parse_face_data(split_line, mesh, field_order):
    face_data = split_line[1:]
    assert len(face_data) == 3, "Non-triangular faces"

    triangle_indexes = []
    for ind_data in face_data:
        split_data = ind_data.split("/")
        assert len(split_data) == len(field_order) + 1, "Not enough face data"

        # TODO: Parse all other fields
        triangle_indexes.append(split_data[0])

    mesh.triangles.append(Vec3(*triangle_indexes, vec_type=int))


PARSE_FUNCS = {
    OBJ_COMMENT_FIELD: _parse_comment,
    OBJ_MESH_NAME_FIELD: _parse_mesh_name,
    OBJ_VERTEX_FIELD: _parse_vertex,
    OBJ_UV_FIELD: _parse_uv,
    OBJ_NORMAL_FIELD: _parse_normal,
    OBJ_SMOOTH_SHADING_FIELD: _parse_smooth,
    OBJ_FACE_DATA_FIELD: _parse_face_data
}
