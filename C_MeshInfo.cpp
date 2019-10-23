#include "C_MeshInfo.h"
#include "MeshObject.h"

C_MeshInfo::C_MeshInfo(GameObject* gameobject, Component_Type type) : Component(type, gameobject)
{
	name = "Mesh Info";
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
	//ImGui::PushID("active_MeshInfo"); ImGui::Checkbox("", &active); ImGui::PopID(); ImGui::SameLine();
	if (ImGui::CollapsingHeader(name.c_str(), open_mesh_info))
	{
		ImGui::Text("Vertices: %i", gameobject->mesh->vertices.size()); ImGui::SameLine(); ImGui::Spacing();  ImGui::SameLine(); ImGui::Text("Faces: %i", gameobject->mesh->indices.size() / 3);
		ImGui::Checkbox("Boundary Box", &gameobject->boundary_box);
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
