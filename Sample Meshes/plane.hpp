#ifndef PLANE_MESH_HPP
#define PLANE_MESH_HPP

#include "mesh.hpp"

VertexData planeVertexData[]{
	VertexData{Vec3f{1.5f, -1.0f, 1.5f}, Vec3f{-0.0f, 1.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}},
	VertexData{Vec3f{-1.5f, -1.0f, -1.5f}, Vec3f{-0.0f, 1.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}},
	VertexData{Vec3f{-1.5f, -1.0f, 1.5f}, Vec3f{-0.0f, 1.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}},
	VertexData{Vec3f{1.5f, -1.0f, -1.5f}, Vec3f{0.0f, 1.0f, 0.0f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}}};

Vec3i planeTriangles[]{
	Vec3i{0, 1, 2},
	Vec3i{0, 3, 1}};

Mesh planeMesh{4, 2, planeVertexData, planeTriangles};

#endif