#pragma once
#include "Globals.h"
#include "Application.h"

class aiMesh;

class MeshObject
{
public:
	MeshObject();
	~MeshObject();

	bool Load(aiMesh * new_mesh);

	void CleanUp();

public:
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* indices = nullptr;

	uint id_vertex = 0;
	uint num_vertex = 0;
	float* vertices = nullptr;

	uint num_normal = 0;
	float* normals = nullptr;

	uint id_color = 0;
	uint num_color = 0;
	float* colors = nullptr;
};

