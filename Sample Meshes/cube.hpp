#ifndef CUBE_HPP
#define CUBE_HPP

#include "mesh.hpp"

RGBA white{255, 255, 255, 255};

size_t cubeVertexCount{8};
size_t cubeTriangleCount{12};

VertexData cubeVertexData[]{
	VertexData{Vec3f{1.0f, 1.0f, -1.0f}, white},
	VertexData{Vec3f{1.0f, -1.0f, -1.0f}, white},
	VertexData{Vec3f{1.0f, 1.0f, 1.0f}, white},
	VertexData{Vec3f{1.0f, -1.0f, 1.0f}, white},
	VertexData{Vec3f{-1.0f, 1.0f, -1.0f}, white},
	VertexData{Vec3f{-1.0f, -1.0f, -1.0f}, white},
	VertexData{Vec3f{-1.0f, 1.0f, 1.0f}, white},
	VertexData{Vec3f{-1.0f, -1.0f, 1.0f}, white}};

Vec3i cubeTriangles[]{
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

Mesh cubeMesh{cubeVertexCount, cubeTriangleCount, cubeVertexData, cubeTriangles};

#endif