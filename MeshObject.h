#ifndef __MESHOBJECT_H__
#define __MESHOBJECT_H__
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Mesh_R.h"
#include "Joint.h"


struct Vertex {
	vec3     Position;
	vec3     Normal;
	vec4     Colors;
	vec2     TexCoords;

	vec4     Joints;
	vec4     Weights;
};

struct VertexBoneData
{
	uint BoneID;
	uint indices;
	float weights;
};

class aiMesh;

class MeshObject
{
public:
	MeshObject(vector<Vertex> vertices, vector<uint> indices, GameObject* gameObject);
	~MeshObject();

	bool SetupBuffers();

	void Draw();

	const vec3 getNormal(vec3 p1, vec3 p2, vec3 p3) const;

	void CleanUp();

	void DebugNormals() const;

public:
	vector<Vertex> vertices;
	vector<uint> indices;
	uint VAO;
	GameObject* gameobject;
private:
	uint VBO, EBO;
};
#endif
