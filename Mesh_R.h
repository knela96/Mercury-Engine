#ifndef MESH_R_H_
#define MESH_R_H_

#include "Resources.h"

enum Buffers
{
	indices_size,
	vertices_size,
	normals_size,
	tex_coords_size,
	buffer_size
};

class Mesh_R : public Resources
{
public:
	Mesh_R() : Resources(MeshR) {}
	~Mesh_R() {}

public:
	uint buffersSize[buffer_size];

	uint*	_indices = nullptr;
	float*	_vertices = nullptr;
	float*	_normals = nullptr;
	float*	_tex_coords = nullptr;
};
#endif

