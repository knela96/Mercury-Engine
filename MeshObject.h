#pragma once
#include "Globals.h"
#include "Application.h"

class aiMesh;
enum aiTextureType;

struct Vertex {
	vec3 Position;
	vec3 Normal;
	vec4 Colors;
	vec2 TexCoords;
};

struct Texture {
	uint id; 
	string path;
	aiTextureType type;
};

class MeshObject
{
public:
	MeshObject(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	~MeshObject();

	bool SetupBuffers();

	void Draw();

	char* getType(aiTextureType type);

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

