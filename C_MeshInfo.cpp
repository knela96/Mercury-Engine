#include "C_MeshInfo.h"
#include "MeshObject.h"

C_MeshInfo::C_MeshInfo(GameObject* gameobject, Component_Type type) : Component(type, gameobject)
{
	name = "Mesh Info";
	ID = App->RandomNumberGenerator.GetIntRNInRange();
}

C_MeshInfo::~C_MeshInfo()
{
}

bool C_MeshInfo::Enable()
{
	active = true;
	return true;
}

void C_MeshInfo::Update()
{
	if (ImGui::CollapsingHeader(name.c_str(), open_mesh_info))
	{
		int vs = 0;
		int is = 0;
		if (gameobject->mesh != nullptr) {
			vs = gameobject->mesh->vertices.size();
			is = gameobject->mesh->indices.size() / 3;
		}
		ImGui::Text("Vertices: %i", vs); ImGui::SameLine(); ImGui::Spacing();  ImGui::SameLine(); ImGui::Text("Tris: %i", is);
		ImGui::Checkbox("Boundary Box", &gameobject->boundary_box);
		if (ImGui::IsItemActivated()) { gameobject->UpdateBox(); }
	}

	if (!unFold) {
		ImGui::GetStateStorage()->SetInt(ImGui::GetID(name.c_str()), 1);
		unFold = true;
	}
}

bool C_MeshInfo::Disable()
{
	active = false;
	return true;
}

void C_MeshInfo::Save(const char * _name, json & file)
{
	file["Game Objects"][_name]["Components"]["Mesh"]["Active"] = gameobject->boundary_box;
}

void C_MeshInfo::Load(const char * _name, const json & file)
{
	gameobject->boundary_box = file["Game Objects"][_name]["Components"]["Mesh"]["Active"].get<bool>();
}
