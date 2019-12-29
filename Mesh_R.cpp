#include "Mesh_R.h"
#include "MeshObject.h"

std::vector<Vertex> Mesh_R::toVertex()
{
	float color = 1.0f;
	vector<Vertex> vec;
	Vertex vertex;
	map<uint, VertexBoneData> map;
	uint pos = 0;

	for (uint i = 0; i < buffersSize[vertices_size]; ++i)
	{
		if (buffersSize[vertices_size] > 0) {
			memcpy(&vertex.Position.x, &_vertices[i * 3], sizeof(float));
			memcpy(&vertex.Position.y, &_vertices[i * 3 + 1], sizeof(float));
			memcpy(&vertex.Position.z, &_vertices[i * 3 + 2], sizeof(float));
		}

		if (buffersSize[normals_size] > 0) {
			memcpy(&vertex.Normal.x, &_normals[i * 3], sizeof(float));
			memcpy(&vertex.Normal.y, &_normals[i * 3 + 1], sizeof(float));
			memcpy(&vertex.Normal.z, &_normals[i * 3 + 2], sizeof(float));
		}

		if (buffersSize[vertices_size] > 0) {
			memcpy(&vertex.Colors.x, &color, sizeof(float));
			memcpy(&vertex.Colors.y, &color, sizeof(float));
			memcpy(&vertex.Colors.z, &color, sizeof(float));
		}

		if (buffersSize[tex_coords_size] > 0) {
			memcpy(&vertex.TexCoords.x, &_tex_coords[i * 2], sizeof(float));
			memcpy(&vertex.TexCoords.y, &_tex_coords[i * 2 + 1], sizeof(float));
		}
		else {
			memcpy(&vertex.TexCoords.x, &color, sizeof(float));
			memcpy(&vertex.TexCoords.y, &color, sizeof(float));
		}

		vec.push_back(vertex);
	}
	uint temp[4] = { 0,0,0,0 };
	float temp2[4] = { 0,0,0,0 };
	uint current = _weights_indices[0];
	uint first = 0;
	uint index = 0;
	uint steps = 0;
	for (int i = 0; i < vec.size(); ++i) {
		steps = 0;
		while (current == _weights_indices[index] && index < buffersSize[weights_size]) {
			index++;
			steps++;
		}
		for (int i = 0; i < steps; ++i) {
			temp[i] = _bones[first];
			temp2[i] = _weights[first];
			first++;
		}
		vec.at(i).Joints =  vec4(temp[0], temp[1], temp[2] ,temp[3]);
		vec.at(i).Weights =  vec4(temp2[0], temp2[1], temp2[2] ,temp2[3]);
		current = _weights_indices[index];
		temp[0] = temp[1] = temp[2] = temp[3] = 0;
		temp2[0] = temp2[1] = temp2[2] = temp2[3] = 0;
		first = index;
	}


	return vec;
}
