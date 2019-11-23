#ifndef __MESHOBJECT_H__
#define __MESHOBJECT_H__
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Mesh_R.h"

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

	vector<Vertex> vertices;
	vector<uint> indices;

	Mesh_R* mesh;

	uint VAO;

private:
	uint VBO, EBO;
};
#endif