#include "GameObject.h"
#include "Application.h"
#include "MeshObject.h"
#include "C_Transform.h"
#include "C_Normals.h"
#include "C_MeshInfo.h"
#include "C_Material.h"

GameObject::GameObject(string name) : name(name){
	components.push_back(AddComponent(Transform));
}

GameObject::GameObject(MeshObject* mesh, vector<Texture*> textures, string name) : mesh(mesh), textures(textures), name(name) {
	components.push_back(AddComponent(Transform));
	components.push_back(AddComponent(Mesh_Info));
	components.push_back(AddComponent(Normals));
	components.push_back(AddComponent(Material));
}

GameObject::~GameObject() {}


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
