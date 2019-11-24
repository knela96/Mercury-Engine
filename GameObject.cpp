#include "GameObject.h"
#include "Application.h"
#include "MeshObject.h"
#include "C_Transform.h"
#include "C_Normals.h"
#include "C_MeshInfo.h"
#include "C_Material.h"
#include "C_Camera.h"

GameObject::GameObject(string name, GameObject * parent) : name(name), parent(parent){
	AddComponent(Transform);
	ID = App->RandomNumberGenerator.GetIntRNInRange();
}

GameObject::GameObject(MeshObject* mesh, vector<Texture*> textures, string name) : mesh(mesh), textures(textures), name(name) {
	AddComponent(Transform);
	AddComponent(Mesh_Info);
	AddComponent(Normals);
	AddComponent(Material);
	ID = App->RandomNumberGenerator.GetIntRNInRange();
}

GameObject::~GameObject() {}

bool GameObject::Start()
{
	if (mesh != nullptr) {
		App->scene_intro->AddAABB(&aabb, Color(0.0f, 1.0f, 0.0f, 1.0f));
		App->scene_intro->AddOBB(&obb, Color(0.0f, 0.0f, 1.0f, 1.0f));

		/*App->scene_intro->AddAABB(&aabb, mesh->b_aabb->color);
		App->scene_intro->AddOBB(&obb, mesh->b_obb->color);*/

		mesh->UpdateBox();
	}
	return false;
}

void GameObject::StartChilds() {
	for (int i = 0; i < childs.size(); ++i) {
		if (active)
			childs[i]->Start();
		childs[i]->StartChilds();
	}
}

void GameObject::UpdateChilds() {
	for (int i = 0; i < childs.size(); ++i) {
		childs[i]->active = active;
		childs[i]->transform->UpdateMatrices();
		childs[i]->UpdateChilds();
	}
}

void GameObject::drawChilds() {
	for(int i = 0; i < childs.size(); ++i){
		if (active) {
			if (App->scene_intro->main_camera->camera->CullFace(childs[i]))
				childs[i]->Draw();
		}
			childs[i]->drawChilds();
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
	components.push_back(component);
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

void GameObject::Save(const char* _name, json& file) {
	file["Game Objects"][_name]["Name"] = name;
	file["Game Objects"][_name]["UID"] = ID;
	if (parent != nullptr) {
		file["Game Objects"][_name]["Parent UID"] = parent->ID;
	}
	else {
		file["Game Objects"][_name]["Parent UID"] = 0;
	}
	file["Game Objects"][_name]["Enable"] = active;
	file["Game Objects"][_name]["Static"] = _static;

	for (uint i = 0; i < components.size(); ++i)
		components[i]->Save(_name, file);
}

void GameObject::Load(const char * _name, const json & file)
{
	name = file["Game Objects"][_name]["Name"].get<string>().c_str();
	ID = file["Game Objects"][_name]["UID"].get<uint>();
	if (file["Game Objects"][_name].find("Parent UID") != file["Game Objects"][_name].end()) {
		int id = file["Game Objects"][_name]["Parent UID"].get<uint>();
		App->scene_intro->setParentByID(id, App->scene_intro->root, this);
	}
	active = file["Game Objects"][_name]["Enable"].get<bool>();
	_static = file["Game Objects"][_name]["Static"].get<bool>();

	if(file["Game Objects"][_name]["Components"].find("Transform") != file["Game Objects"][_name]["Components"].end());
	{
		transform->Load(_name, file);
	}

	if (file["Game Objects"][_name]["Components"].find("Mesh") != file["Game Objects"][_name]["Components"].end())
	{
		C_MeshInfo* t = (C_MeshInfo*)AddComponent(Component_Type::Mesh_Info);
		t->Load(_name, file);
	}
	
	if (file["Game Objects"][_name]["Components"].find("Normals") != file["Game Objects"][_name]["Components"].end())
	{
		C_Normals* t = (C_Normals*)AddComponent(Component_Type::Normals);
		t->Load(_name, file);
	}

	if (file["Game Objects"][_name]["Components"].find("Material") != file["Game Objects"][_name]["Components"].end())
	{
		C_Material* t = (C_Material*)AddComponent(Component_Type::Material);
		t->Load(_name, file);
	}

	if (file["Game Objects"][_name]["Components"].find("Camera") != file["Game Objects"][_name]["Components"].end())
	{
		C_Camera* t = (C_Camera*)AddComponent(Component_Type::Camera);
		t->Load(_name, file);
		App->scene_intro->main_camera = this;
	}
	/*std::string
	App->importer->LoadFile("/Library/Meshes/")*/

}