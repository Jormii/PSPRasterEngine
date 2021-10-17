#ifndef CUBE_MESH_HPP
#define CUBE_MESH_HPP

#include "mesh.hpp"

VertexData cubeVertexData[]{
	VertexData{Vec3f{-1.0f, 1.0f, -1.0f}, Vec3f{0.0f, 1.0f, -0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.875f, 0.5f}},
	VertexData{Vec3f{1.0f, 1.0f, 1.0f}, Vec3f{0.0f, 1.0f, -0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.625f, 0.75f}},
	VertexData{Vec3f{1.0f, 1.0f, -1.0f}, Vec3f{0.0f, 1.0f, -0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.625f, 0.5f}},
	VertexData{Vec3f{1.0f, 1.0f, 1.0f}, Vec3f{0.0f, -0.0f, 1.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.625f, 0.75f}},
	VertexData{Vec3f{-1.0f, -1.0f, 1.0f}, Vec3f{0.0f, -0.0f, 1.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.375f, 1.0f}},
	VertexData{Vec3f{1.0f, -1.0f, 1.0f}, Vec3f{0.0f, -0.0f, 1.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.375f, 0.75f}},
	VertexData{Vec3f{-1.0f, 1.0f, 1.0f}, Vec3f{-1.0f, 0.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.625f, 0.0f}},
	VertexData{Vec3f{-1.0f, -1.0f, -1.0f}, Vec3f{-1.0f, 0.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.375f, 0.25f}},
	VertexData{Vec3f{-1.0f, -1.0f, 1.0f}, Vec3f{-1.0f, 0.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.375f, 0.0f}},
	VertexData{Vec3f{1.0f, -1.0f, -1.0f}, Vec3f{0.0f, -1.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.375f, 0.5f}},
	VertexData{Vec3f{-1.0f, -1.0f, 1.0f}, Vec3f{0.0f, -1.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.125f, 0.75f}},
	VertexData{Vec3f{-1.0f, -1.0f, -1.0f}, Vec3f{0.0f, -1.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.125f, 0.5f}},
	VertexData{Vec3f{1.0f, 1.0f, -1.0f}, Vec3f{1.0f, -0.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.625f, 0.5f}},
	VertexData{Vec3f{1.0f, -1.0f, 1.0f}, Vec3f{1.0f, -0.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.375f, 0.75f}},
	VertexData{Vec3f{1.0f, -1.0f, -1.0f}, Vec3f{1.0f, -0.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.375f, 0.5f}},
	VertexData{Vec3f{-1.0f, 1.0f, -1.0f}, Vec3f{0.0f, 0.0f, -1.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.625f, 0.25f}},
	VertexData{Vec3f{1.0f, -1.0f, -1.0f}, Vec3f{0.0f, 0.0f, -1.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.375f, 0.5f}},
	VertexData{Vec3f{-1.0f, -1.0f, -1.0f}, Vec3f{0.0f, 0.0f, -1.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.375f, 0.25f}},
	VertexData{Vec3f{-1.0f, 1.0f, 1.0f}, Vec3f{0.0f, 1.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.875f, 0.75f}},
	VertexData{Vec3f{-1.0f, 1.0f, 1.0f}, Vec3f{0.0f, 0.0f, 1.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.625f, 1.0f}},
	VertexData{Vec3f{-1.0f, 1.0f, -1.0f}, Vec3f{-1.0f, 0.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.625f, 0.25f}},
	VertexData{Vec3f{1.0f, -1.0f, 1.0f}, Vec3f{0.0f, -1.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.375f, 0.75f}},
	VertexData{Vec3f{1.0f, 1.0f, 1.0f}, Vec3f{1.0f, -0.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.625f, 0.75f}},
	VertexData{Vec3f{1.0f, 1.0f, -1.0f}, Vec3f{0.0f, 0.0f, -1.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}, Vec2f{0.625f, 0.5f}}};

Vec3i cubeTriangles[]{
	Vec3i{0, 1, 2},
	Vec3i{3, 4, 5},
	Vec3i{6, 7, 8},
	Vec3i{9, 10, 11},
	Vec3i{12, 13, 14},
	Vec3i{15, 16, 17},
	Vec3i{0, 18, 1},
	Vec3i{3, 19, 4},
	Vec3i{6, 20, 7},
	Vec3i{9, 21, 10},
	Vec3i{12, 22, 13},
	Vec3i{15, 23, 16}};

Mesh cubeMesh{24, 12, cubeVertexData, cubeTriangles};

#endif