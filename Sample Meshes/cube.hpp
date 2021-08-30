#include <vector>

#include "vec3f.hpp"
#include "vec3i.hpp"
#include "rgba.hpp"
#include "mesh.hpp"

std::vector<Vec3f> cubeVertices{
	Vec3f{1.0f, 1.0f, -1.0f},
	Vec3f{1.0f, -1.0f, -1.0f},
	Vec3f{1.0f, 1.0f, 1.0f},
	Vec3f{1.0f, -1.0f, 1.0f},
	Vec3f{-1.0f, 1.0f, -1.0f},
	Vec3f{-1.0f, -1.0f, -1.0f},
	Vec3f{-1.0f, 1.0f, 1.0f},
	Vec3f{-1.0f, -1.0f, 1.0f}};

std::vector<Vec3i> cubeTriangles{
	Vec3i{4, 2, 0},
	Vec3i{2, 7, 3},
	Vec3i{6, 5, 7},
	Vec3i{1, 7, 5},
	Vec3i{0, 3, 1},
	Vec3i{4, 1, 5},
	Vec3i{4, 6, 2},
	Vec3i{2, 6, 7},
	Vec3i{6, 4, 5},
	Vec3i{1, 3, 7},
	Vec3i{0, 2, 3},
	Vec3i{4, 0, 1}};

std::vector<RGBA> cubeColor{
	RGBA{255, 255, 255, 255},
	RGBA{255, 255, 255, 255},
	RGBA{255, 255, 255, 255},
	RGBA{255, 255, 255, 255},
	RGBA{255, 255, 255, 255},
	RGBA{255, 255, 255, 255},
	RGBA{255, 255, 255, 255},
	RGBA{255, 255, 255, 255}};

Mesh cubeMesh{cubeVertices, cubeTriangles, cubeColor};
