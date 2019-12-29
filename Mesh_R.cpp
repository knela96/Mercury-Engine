#include "Mesh_R.h"
#include "MeshObject.h"

std::vector<Vertex> Mesh_R::toVertex()
{
	float color = 1.0f;
	vector<Vertex> vec;
	Vertex vertex;
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

	return vec;
}
