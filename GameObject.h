#pragma once
#include "Globals.h"
#include "Application.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "lib/Assimp/libx86/assimp.lib")


struct Texture {
	uint id;
	string path;
	aiTextureType type;
	vec2 size;
};

class MeshObject;

enum aiTextureType;

class GameObject
{
public:
	GameObject(MeshObject* mesh, vector<Texture*> textures, string name);
	~GameObject();

	virtual void Draw() {}

	void drawChilds();

	void CleanUp();

	char* getType(aiTextureType type);



public:
	MeshObject* mesh;
	vector<Texture*> textures;
	vector<GameObject*> childs;

	vec3 position;
	vec3 rotation;
	vec3 scale;

	bool face_normals;
	bool vertex_normals;

	string name;

	FileFormats c_texformat = PNG;

	bool debug_tex = false;

	bool active = true;
};
