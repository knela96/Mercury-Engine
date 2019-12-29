#ifndef MESH_R_H_
#define MESH_R_H_

#include "Globals.h"
#include "Application.h"
#include "Resources.h"


enum Buffers
{
	indices_size,
	vertices_size,
	normals_size,
	tex_coords_size,
	weights_size,
	bones_size,
	buffer_size
};

struct Vertex;

class Mesh_R : public Resources
{
public:
	Mesh_R() : Resources(MeshR) {}
	~Mesh_R() {}

	std::vector<Vertex> toVertex();

public:
	uint buffersSize[buffer_size];

	uint*	_indices = nullptr;
	float*	_vertices = nullptr;
	float*	_normals = nullptr;
	float*	_tex_coords = nullptr;
	uint*	bones_size;
	float*	_weights = nullptr;
	float*	_weights_indices = nullptr;
};
#endif

