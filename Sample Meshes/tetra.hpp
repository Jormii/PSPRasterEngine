#ifndef TETRA_MESH_HPP
#define TETRA_MESH_HPP

#include "mesh.hpp"

VertexData tetraVertexData[]{
	VertexData{Vec3f{0.0f, -1.0f, -1.0f}, Vec3f{0.8401679992675781f, 0.24253599345684052f, -0.48507100343704224f}, Vec4f{0.0f, 1.0f, 0.0f, 1.0f}},
	VertexData{Vec3f{0.0f, 1.0f, 0.0f}, Vec3f{0.8401679992675781f, 0.24253599345684052f, -0.48507100343704224f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}},
	VertexData{Vec3f{0.8660249710083008f, -1.0f, 0.5f}, Vec3f{0.8401679992675781f, 0.24253599345684052f, -0.48507100343704224f}, Vec4f{1.0f, 0.0f, 0.0f, 1.0f}},
	VertexData{Vec3f{0.0f, -1.0f, -1.0f}, Vec3f{0.0f, -1.0f, -0.0f}, Vec4f{0.0f, 1.0f, 0.0f, 1.0f}},
	VertexData{Vec3f{0.8660249710083008f, -1.0f, 0.5f}, Vec3f{0.0f, -1.0f, -0.0f}, Vec4f{1.0f, 0.0f, 0.0f, 1.0f}},
	VertexData{Vec3f{-0.8660249710083008f, -1.0f, 0.5f}, Vec3f{0.0f, -1.0f, -0.0f}, Vec4f{1.0f, 0.0f, 0.0f, 1.0f}},
	VertexData{Vec3f{0.8660249710083008f, -1.0f, 0.5f}, Vec3f{-0.0f, 0.24253599345684052f, 0.9701430201530457f}, Vec4f{1.0f, 0.0f, 0.0f, 1.0f}},
	VertexData{Vec3f{0.0f, 1.0f, 0.0f}, Vec3f{-0.0f, 0.24253599345684052f, 0.9701430201530457f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}},
	VertexData{Vec3f{-0.8660249710083008f, -1.0f, 0.5f}, Vec3f{-0.0f, 0.24253599345684052f, 0.9701430201530457f}, Vec4f{1.0f, 0.0f, 0.0f, 1.0f}},
	VertexData{Vec3f{-0.8660249710083008f, -1.0f, 0.5f}, Vec3f{-0.8401679992675781f, 0.24253599345684052f, -0.48507100343704224f}, Vec4f{1.0f, 0.0f, 0.0f, 1.0f}},
	VertexData{Vec3f{0.0f, 1.0f, 0.0f}, Vec3f{-0.8401679992675781f, 0.24253599345684052f, -0.48507100343704224f}, Vec4f{1.0f, 1.0f, 1.0f, 1.0f}},
	VertexData{Vec3f{0.0f, -1.0f, -1.0f}, Vec3f{-0.8401679992675781f, 0.24253599345684052f, -0.48507100343704224f}, Vec4f{0.0f, 1.0f, 0.0f, 1.0f}}};

Vec3i tetraTriangles[]{
	Vec3i{0, 1, 2},
	Vec3i{3, 4, 5},
	Vec3i{6, 7, 8},
	Vec3i{9, 10, 11}};

Mesh tetraMesh{12, 4, tetraVertexData, tetraTriangles};

#endif