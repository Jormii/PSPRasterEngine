#ifndef TETRA_MESH_HPP
#define TETRA_MESH_HPP

#include "mesh.hpp"

VertexData tetraVertexData[]{
	VertexData{Vec3f{0.0f, -1.0f, -1.0f}, Vec3f{0.8401679992675781f, 0.24253599345684052f, -0.48507100343704224f}, RGBA{0, 255, 0, 255}.AsVec4f()},
	VertexData{Vec3f{0.0f, 1.0f, 0.0f}, Vec3f{0.8401679992675781f, 0.24253599345684052f, -0.48507100343704224f}, RGBA{255, 251, 251, 255}.AsVec4f()},
	VertexData{Vec3f{0.8660249710083008f, -1.0f, 0.5f}, Vec3f{0.8401679992675781f, 0.24253599345684052f, -0.48507100343704224f}, RGBA{255, 0, 0, 255}.AsVec4f()},
	VertexData{Vec3f{0.0f, -1.0f, -1.0f}, Vec3f{0.0f, -1.0f, -0.0f}, RGBA{0, 255, 0, 255}.AsVec4f()},
	VertexData{Vec3f{0.8660249710083008f, -1.0f, 0.5f}, Vec3f{0.0f, -1.0f, -0.0f}, RGBA{255, 0, 0, 255}.AsVec4f()},
	VertexData{Vec3f{-0.8660249710083008f, -1.0f, 0.5f}, Vec3f{0.0f, -1.0f, -0.0f}, RGBA{255, 0, 0, 255}.AsVec4f()},
	VertexData{Vec3f{0.8660249710083008f, -1.0f, 0.5f}, Vec3f{-0.0f, 0.24253599345684052f, 0.9701430201530457f}, RGBA{255, 0, 0, 255}.AsVec4f()},
	VertexData{Vec3f{0.0f, 1.0f, 0.0f}, Vec3f{-0.0f, 0.24253599345684052f, 0.9701430201530457f}, RGBA{255, 251, 251, 255}.AsVec4f()},
	VertexData{Vec3f{-0.8660249710083008f, -1.0f, 0.5f}, Vec3f{-0.0f, 0.24253599345684052f, 0.9701430201530457f}, RGBA{255, 0, 0, 255}.AsVec4f()},
	VertexData{Vec3f{-0.8660249710083008f, -1.0f, 0.5f}, Vec3f{-0.8401679992675781f, 0.24253599345684052f, -0.48507100343704224f}, RGBA{255, 0, 0, 255}.AsVec4f()},
	VertexData{Vec3f{0.0f, 1.0f, 0.0f}, Vec3f{-0.8401679992675781f, 0.24253599345684052f, -0.48507100343704224f}, RGBA{255, 251, 251, 255}.AsVec4f()},
	VertexData{Vec3f{0.0f, -1.0f, -1.0f}, Vec3f{-0.8401679992675781f, 0.24253599345684052f, -0.48507100343704224f}, RGBA{0, 255, 0, 255}.AsVec4f()}};

Vec3i tetraTriangles[]{
	Vec3i{0, 1, 2},
	Vec3i{3, 4, 5},
	Vec3i{6, 7, 8},
	Vec3i{9, 10, 11}};

Mesh tetraMesh{12, 4, tetraVertexData, tetraTriangles};

#endif