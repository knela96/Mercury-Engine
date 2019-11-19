#include "GameObject.h"
#include "Application.h"
#include "MeshObject.h"
#include "C_Transform.h"
#include "C_Normals.h"
#include "C_MeshInfo.h"
#include "C_Material.h"
#include "C_Camera.h"

GameObject::GameObject(string name, GameObject * parent) : name(name), parent(parent){
	components.push_back(AddComponent(Transform));
	Start();
}

GameObject::GameObject(MeshObject* mesh, vector<Texture*> textures, string name) : mesh(mesh), textures(textures), name(name) {
	components.push_back(AddComponent(Transform));
	components.push_back(AddComponent(Mesh_Info));
	components.push_back(AddComponent(Normals));
	components.push_back(AddComponent(Material)); 
	Start();
}

GameObject::~GameObject() {}

bool GameObject::Start()
{
	if (mesh != nullptr) {
		App->scene_intro->AddAABB(&aabb, Color(0.0f, 1.0f, 0.0f, 1.0f));
		App->scene_intro->AddOBB(&obb, Color(0.0f, 0.0f, 1.0f, 1.0f));
	}
	return false;
}

void GameObject::StartChilds() {
	if (active) {
		for (int i = 0; i < childs.size(); ++i) {
			childs[i]->Start();
			childs[i]->StartChilds();
		}
	}
}

void GameObject::UpdateChilds() {
	if (active) {
		for (int i = 0; i < childs.size(); ++i) {
			childs[i]->transform->UpdateMatrices();
			childs[i]->UpdateChilds();
		}
	}
}

void GameObject::drawChilds() {
	if (active) {
		for(int i = 0; i < childs.size(); ++i){
				childs[i]->Draw();
				childs[i]->drawChilds();
		}
	}
}

float4x4 GameObject::mat2float4(mat4x4 mat)
{
	float4x4 f_mat;
	f_mat.Set(mat.M);
	return f_mat.Transposed();
}

void GameObject::CleanUp() {
	for (GameObject* obj : childs) {
		if (obj != nullptr) {
			obj->CleanUp();
		}
		obj = nullptr;
	}
	childs.clear();

	for (int i = 0; i < components.size(); ++i) {
		if (components[i] != nullptr)
			delete components[i];
		components[i] = nullptr;
	}
	components.clear();


	for (int i = 0; i < textures.size(); ++i) {
		textures[i] = nullptr;
	}
	textures.clear();

	if (mesh != nullptr) {
		mesh->CleanUp();
		delete mesh;
	}
	mesh = nullptr;
}


void GameObject::UpdateBox() {
	obb = box;
	obb.Transform(mat2float4(this->transform->globalMatrix));
	aabb.SetNegativeInfinity();
	aabb.Enclose(obb);
}

const char* GameObject::getType(aiTextureType type)
{
	switch (type) {
	case aiTextureType_DIFFUSE:
		return "Diffuse_Map";
		break;
	case aiTextureType_SPECULAR:
		return "Specular_Map";
		break;
	case aiTextureType_NORMALS:
		return "Normal_Map";
		break;
	case aiTextureType_HEIGHT:
		return "Height_Map";
		break;
	}
}

Component * GameObject::AddComponent(Component_Type type)
{
	Component* component;
	switch (type) {
	case Component_Type::Transform:
		component = new C_Transform(this, type);
		transform = (C_Transform*)component;
		break;
	case Component_Type::Mesh_Info:
		component = new C_MeshInfo(this, type);
		break;
	case Component_Type::Normals:
		component = new C_Normals(this, type);
		normals = (C_Normals*) component;
		break;
	case Component_Type::Material:
		component = new C_Material(this, type);
		break;
	case Component_Type::Script:
		//component = new C_Script(this, type);
		break;
	case Component_Type::Camera:
		component = new C_Camera(this, type);
		camera = (C_Camera*)component;
		break;
	}

	component->Enable();

	return component;
}

Component * GameObject::getComponent(Component_Type type)
{
	for (int i = 0; i < components.size(); ++i) {
		if (components[i]->type == type)
			return components[i];
	}
	return nullptr;
}
