#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_
#include "Globals.h"
#include "Application.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "MathGeoLib/include/MathGeoLib.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "lib/Assimp/libx86/assimp.lib")
#ifdef _DEBUG
#pragma comment (lib, "lib/MathGeoLib/libx86/Debug/MathGeoLib.lib")
#else
#pragma comment (lib, "lib/MathGeoLib/libx86/Release/MathGeoLib.lib")
#endif


struct Texture {
	uint id;
	string path;
	aiTextureType type;
	vec2 size;
};

class MeshObject;
class Component;
class C_Normals;
class C_Transform;
class C_Camera;

enum aiTextureType;
enum Component_Type;

class GameObject
{
public:
	GameObject(string name,GameObject* parent = nullptr);
	GameObject();
	~GameObject();

	virtual void Draw() {}

	bool Start();
	void StartChilds();
	void UpdateChilds();

	void drawChilds();

	static float4x4 mat2float4(mat4x4 mat);

	void CleanUp();

	void UpdateBox();

	const char* getType(aiTextureType type);

	Component* AddComponent(Component_Type type);

	Component* getComponent(Component_Type type);

	
public:
	MeshObject* mesh = nullptr;
	vector<Texture*> textures;
	vector<Component*> components;
	vector<GameObject*> childs;

	GameObject* parent = nullptr;

	bool face_normals = false;
	bool vertex_normals = false;

	string name;
	FileFormats c_texformat = PNG;
	bool debug_tex = false;
	bool active = true;
	bool selected = false;
	bool _static = false;

	AABB box;
	AABB aabb;
	OBB obb;
	bool boundary_box = false;


public:
	C_Transform* transform = nullptr;
	C_Normals* normals = nullptr;
	C_Camera* camera = nullptr;


};

#endif GAMEOBJECT_H_