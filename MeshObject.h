#ifndef __MESHOBJECT_H__
#define __MESHOBJECT_H__
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"

enum Buffers
{
	indices_size,
	vertices_size,
	normals_size,
	tex_coords_size,
	buffer_size
};

struct Vertex {
	vec3 Position;
	vec3 Normal;
	//vec4 Colors;
	vec2 TexCoords;
};

class aiMesh;

class MeshObject : public GameObject
{
public:
	MeshObject();
	~MeshObject();

	bool SetupBuffers();

	void Transform2Vertex();

	void Draw();

	const vec3 getNormal(vec3 p1, vec3 p2, vec3 p3) const;

	void CleanUp();

	void DebugNormals() const;

public:
	uint buffersSize[buffer_size];

	uint*	_indices = nullptr;
	float*	_vertices = nullptr;
	float*	_normals = nullptr;
	float*	_tex_coords = nullptr;

	vector<Vertex> vertices;
	vector<uint> indices;

	uint ID = 0;

	std::string resource_path = "";
	std::string original_path = "";

	uint VAO;

private:
	uint VBO, EBO;
};
#endif