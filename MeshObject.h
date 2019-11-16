#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "MathGeoLib/include/MathGeoLib.h"
#ifdef _DEBUG
#pragma comment (lib, "lib/MathGeoLib/libx86/Debug/MathGeoLib.lib")
#else
#pragma comment (lib, "lib/MathGeoLib/libx86/Release/MathGeoLib.lib")
#endif
struct Vertex {
	vec3 Position;
	vec3 Normal;
	vec4 Colors;
	vec2 TexCoords;
};

template <class T>
struct Box {
	Box(T* box, Color color): box(box), color(color){}
	
	T* box;
	Color color;
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

	template <class T>
	void DrawBox(Box<T>* box) const;

public:
	vector<Vertex> vertices;
	vector<uint> indices;
	uint VAO;

	Box<AABB>* b_aabb = nullptr;
	Box<OBB>* b_obb = nullptr;

private:
	uint VBO, EBO;
};
