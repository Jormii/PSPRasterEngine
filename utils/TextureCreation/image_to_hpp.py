import sys
import os.path

import matplotlib.pyplot as plt


def image_to_hpp(image_file, name="mytex"):
    print(name)
    image = plt.imread(image_file)
    if "int" in image.dtype.name:
        image = image.astype(float) / 255.0

    this_file_abs_path = os.path.dirname(os.path.abspath(__file__))
    hpp_file = open("{}/{}.hpp".format(this_file_abs_path, name), "w")

    try:
        _write_guard_and_includes(hpp_file, name)
        _write_texture_content(hpp_file, image, name)
        _write_texture_initialization(hpp_file, image, name)

        hpp_file.write("\n#endif")
    except Exception as e:
        print(e)
    finally:
        hpp_file.close()


def _write_guard_and_includes(hpp_file, name):
    define = "{}_TEXTURE_HPP".format(name.upper())

    hpp_file.write("#ifndef {}\n".format(define))
    hpp_file.write("#define {}\n\n".format(define))
    hpp_file.write("#include \"texture.hpp\"\n\n")


def _write_texture_content(hpp_file, image, name):
    height, width, _ = image.shape
    R = image[:, :, 0]
    G = image[:, :, 1]
    B = image[:, :, 2]

    total = 0
    size = height * width

    hpp_file.write("Vec4f {}Map[]{{\n".format(name))
    for i in range(height):
        for j in range(width):
            r = R[i, j]
            g = G[i, j]
            b = B[i, j]

            color_str = "Vec4f{{{}f, {}f, {}f, 1.0f}}".format(r, g, b)
            hpp_file.write("\t{}".format(color_str))

            total += 1
            if total != size:
                hpp_file.write(",\n")

    hpp_file.write("};\n\n")


def _write_texture_initialization(hpp_file, image, name):
    height, width, _ = image.shape

    hpp_file.write("Texture {}Texture{{{}, {}, {}}};\n".format(
        name, width, height, "{}Map".format(name)))


if __name__ == "__main__":
    argc = len(sys.argv)
    if argc == 1:
        exit("No image provided")
    elif argc > 2:
        exit("Expected one argument (target image). Got {} instead".format(argc - 1))

    image_file = sys.argv[1]
    image_to_hpp(image_file)
