#ifndef __MESHOBJECT_H__
#define __MESHOBJECT_H__
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Mesh_R.h"

struct Bone {
	UID ID;
	float BoneWeight;
};

struct Vertex {
	vec3 Position;
	vec3 Normal;
	vec4 Colors;
	vec2 TexCoords;

	//bones that the vertex is atached to, if the vertex can be attached to more bones bc of the
	//model we are using, just add more bones.
	Bone Bones[4];	
};

class aiMesh;

class MeshObject : public GameObject
{
public:
	MeshObject(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture*> textures, string name);
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

private:
	uint VBO, EBO;
};
#endif