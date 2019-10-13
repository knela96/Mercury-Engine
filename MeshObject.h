#pragma once
#include "Globals.h"
#include "Application.h"

class aiMesh;

struct Vertex {
	vec3 Position;
	vec3 Normal;
	vec4 Colors;
	vec3 TexCoords;
};

struct Texture {
	uint id;
	string type;
};

class MeshObject
{
public:
	MeshObject(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	~MeshObject();

	bool SetupBuffers();

	void Draw();

	vec3 getNormal(vec3 p1, vec3 p2, vec3 p3);

	void CleanUp();

public:
	vector<Vertex> vertices;
	vector<uint> indices;
	vector<Texture> textures;
	uint VAO;

private:
	uint VBO, EBO;
};

