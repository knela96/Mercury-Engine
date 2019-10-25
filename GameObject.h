#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_
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
class Component;
class C_Normals;

enum aiTextureType;
enum Component_Type;

class GameObject
{
public:
	GameObject(MeshObject* mesh, vector<Texture*> textures, string name);
	~GameObject();

	virtual void Draw() {}

	void drawChilds();

	void CleanUp();

	const char* getType(aiTextureType type);

	Component* AddComponent(Component_Type type);

	Component* getComponent(Component_Type type);

	
public:
	MeshObject* mesh;
	vector<Texture*> textures;
	vector<Component*> components;
	vector<GameObject*> childs;

	GameObject* parent;

	bool face_normals = false;
	bool vertex_normals = false;

	C_Normals* normals = nullptr;
	string name;
	FileFormats c_texformat = PNG;
	bool debug_tex = false;
	bool active = true;

	AABB box;
	bool boundary_box = false;
};

#endif GAMEOBJECT_H_